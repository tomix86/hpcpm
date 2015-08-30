from pymongo import MongoClient


class Database:  # pylint: disable=too-few-public-methods

    def __init__(self):
        self.client = None
        self.database = None
        self.computation_nodes_collection = None

    def configure(self, config):
        self.client = MongoClient(config.get('host'), int(config.get('port')))
        self.database = self.client[config.get('db')]
        self.computation_nodes_collection = self.database[config.get('computation_nodes_collection')]

    def get_computation_node_info(self, name):
        return self.computation_nodes_collection.find_one({'name': name})

    def replace_computation_node_info(self, name, data):
        return self.computation_nodes_collection.replace_one({"name": name}, data, True)


database = Database()  # pylint: disable=invalid-name
