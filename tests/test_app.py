import mock
import unittest
from flask import request

from hpcpm.api.app import flask_app


class TestApp(unittest.TestCase):
    # pylint: disable=invalid-name
    def create_app(self):
        flask_app.config['TESTING'] = True
        ctx = flask_app.app_context()
        ctx.push()
        return flask_app

    def setUp(self):
        self.app = self.create_app()

    @mock.patch('time.time', mock.Mock(return_value=1234))
    def test_start_end_request(self):
        with self.app.test_client() as client:
            client.get('/api/hpcpm/')
            self.assertEquals(request.method, 'GET')
            self.assertEquals(request.start_time, 1234)
            self.assertEquals(request.end_time, 1234)
