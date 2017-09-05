from datetime import datetime
from typing import Dict

from hpcpm.management import log
from hpcpm.management.helpers import pairwise
# pylint: disable=too-few-public-methods


class Rule:
    def __init__(self, api_requests, node_name, device_id):
        self.node_name = node_name
        self.device_id = device_id
        self.api_requests = api_requests

    def set_limit_on_device(self, rule_params: Dict):
        limit_to_set = rule_params['limit']
        current_limit_response = self.api_requests.get_power_limit_info(self.node_name, self.device_id)
        if current_limit_response.status_code != 200:
            current_limit = None
        else:
            current_limit = int(current_limit_response.json()['power_limit'])
        if limit_to_set == current_limit:
            return
        if limit_to_set is None:
            self.api_requests.delete_power_limit_info(self.node_name, self.device_id)
            return
        self.api_requests.put_power_limit_info(self.node_name, self.device_id, limit_to_set)


class TimeBased(Rule):
    def __init__(self, api_requests, node_name, device_id):
        super().__init__(api_requests, node_name, device_id)
        self.subrules = []

    def proceed(self, rule_params):
        log.debug("proceeding TimeBased rule for %s:%s with params %s", self.node_name, self.device_id, rule_params)
        self._parse_params(rule_params)
        current_minute = datetime.utcnow().replace(second=0, microsecond=0)
        limit_to_set = None
        for subrule in self.subrules:
            if subrule['start'] <= current_minute < subrule['end']:
                limit_to_set = subrule['limit']
                break
        current_limit_response = self.api_requests.get_power_limit_info(self.node_name, self.device_id)
        if current_limit_response.status_code != 200:
            current_limit = None
        else:
            current_limit = int(current_limit_response.json()['power_limit'])
        if limit_to_set == current_limit:
            return
        if limit_to_set is None:
            self.api_requests.delete_power_limit_info(self.node_name, self.device_id)
            return
        self.api_requests.put_power_limit_info(self.node_name, self.device_id, limit_to_set)

    def _parse_params(self, params):
        subrules = []
        try:
            for param in params:
                subrule = {'start': datetime.strptime(param['start'], '%Y-%m-%dT%H:%M'),
                           'end': datetime.strptime(param['end'], '%Y-%m-%dT%H:%M'), 'limit': int(param['limit'])}
                if subrule['end'] <= subrule['start']:
                    raise ValueError('Wrong rule parameters')
                subrules.append(subrule)
            subrules.sort(key=lambda x: x['start'])
            for subrule_pair in pairwise(subrules):
                if subrule_pair[0]['end'] > subrule_pair[1]['start']:
                    raise ValueError('Wrong rule parameters')
        except Exception as ex:
            log.exception(ex)
            raise
        self.subrules = subrules


class HardLimit(Rule):
    def proceed(self, rule_params):
        log.debug("proceeding HardLimit rule for %s:%s with params %s", self.node_name, self.device_id, rule_params)
        self.set_limit_on_device(rule_params)


class Withdrawable(Rule):
    def proceed(self, rule_params: Dict):
        log.debug("proceeding Withdrawable rule for %s:%s with params %s", self.node_name, self.device_id, rule_params)
        current_minute = datetime.utcnow().replace(second=0, microsecond=0)
        deadline_str = rule_params.get("deadline")
        deadline = datetime.strptime(deadline_str, '%Y-%m-%dT%H:%M') if deadline_str else None
        if rule_params.get("withdraw", False) or deadline and deadline < current_minute:
            if rule_params.get("previous_rule"):
                self.api_requests.put_rule_for_device(self.node_name, self.device_id, rule_params["previous_rule"])
            else:
                self.api_requests.delete_rule_for_device(self.node_name, self.device_id)
        self.set_limit_on_device(rule_params)
