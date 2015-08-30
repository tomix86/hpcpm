from pymongo import MongoClient


class Database:  # pylint: disable=too-few-public-methods

    def __init__(self, config):
        self.client = MongoClient(config.get('host'), int(config.get('port')))
        self.database = self.client[config.get('db')]
