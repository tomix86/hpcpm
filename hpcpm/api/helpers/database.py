from pymongo import MongoClient


class Database:  # pylint: disable=too-few-public-methods

    def __init__(self):
        self.client = None
        self.database = None
        self.computation_nodes_collection = None
        self.power_limit_collection = None

    def configure(self, config):
        self.client = MongoClient(config.get('host'), int(config.get('port')))
        self.database = self.client[config.get('db')]
        self.computation_nodes_collection = self.database[config.get('computation_nodes_collection')]
        self.power_limit_collection = self.database[config.get('power_limit_collection')]

    def get_computation_node_info(self, name):
        return self.computation_nodes_collection.find_one({'name': name}, {'_id': False})

    def get_computation_node_info_by_address(self, address, port):  # pylint: disable=invalid-name
        return self.computation_nodes_collection.find_one({'address': address, 'port': port}, {'_id': False})

    def get_power_limit_for_device(self, name, device_id):
        return self.power_limit_collection.find_one({'name': name, 'device_id': device_id}, {'_id': False})

    def get_list_of_nodes(self):
        return list(
            self.computation_nodes_collection.find({}, {'name': True, 'address': True, 'port': True, '_id': False}))

    def replace_computation_node_info(self, name, data):
        return self.computation_nodes_collection.replace_one({'name': name}, data, True)

    def replace_power_limit_for_device(self, name, device_id, power_limit):
        return self.power_limit_collection.replace_one({'name': name, 'device_id': device_id}, power_limit, True)

    def delete_computation_node_info(self, name):
        return self.computation_nodes_collection.find_one_and_delete({'name': name})

    def delete_power_limit_infos(self, name):
        return self.power_limit_collection.delete_many({'name': name})

    def delete_power_limit_info(self, name, device_id):
        return self.power_limit_collection.find_one_and_delete({'name': name, 'device_id': device_id}, {'_id': False})


database = Database()  # pylint: disable=invalid-name
