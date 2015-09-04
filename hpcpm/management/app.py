from datetime import datetime
from apscheduler.schedulers.blocking import BlockingScheduler

from hpcpm.management import log


class App:
    def __init__(self, config):
        self.interval = int(config.get('scheduler', 'interval'))

    def check_and_set_nodes_power_limit(self):
        start_time = datetime.now()
        log.info('check_and_set_nodes_power_limit job started at %s', start_time)

        finish_time = datetime.now()
        log.info('check_and_set_nodes_power_limit job finished at %s, it took %s seconds', finish_time,
                 (finish_time - start_time).total_seconds())

    def run(self):
        sched = BlockingScheduler()
        sched.add_job(self.check_and_set_nodes_power_limit, 'interval', seconds=self.interval)
        sched.start()
