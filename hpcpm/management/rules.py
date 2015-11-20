from hpcpm.management import log

# pylint: disable=too-few-public-methods


class Rule:
    def __init__(self, node_name, device_id):
        self.node_name = node_name
        self.device_id = device_id


class TimeBased(Rule):
    def proceed(self, rule_params):
        log.debug("proceeding TimeBased rule for %s:%s with params %s", self.node_name, self.device_id, rule_params)
