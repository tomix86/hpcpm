import configparser
import unittest
import mock

from hpcpm.api.main import main, prepare_app_configuration, parse_args, create_parser, add_arguments_to_parser, \
    try_to_configure_logging, configure_logging, try_to_parse_config_file, parse_config_file, handle_parsing_error, \
    run_api


class TestMain(unittest.TestCase):
    # pylint: disable=invalid-name

    def setUp(self):
        pass

    def tearDown(self):
        pass

    @mock.patch('hpcpm.api.main.prepare_app_configuration', mock.Mock(return_value=['a', 'b']))
    @mock.patch('hpcpm.api.main.run_api')
    def test_main(self, m_run_api):
        val = main()
        self.assertEqual(val, 0)
        m_run_api.assert_called_once_with('a', 'b')

    @mock.patch('hpcpm.api.main.try_to_configure_logging')
    @mock.patch('hpcpm.api.main.parse_args')
    @mock.patch('hpcpm.api.main.try_to_parse_config_file')
    def test_prepare_app_configuration(self, m_try_to_parse_config_file, m_parse_args, m_try_to_configure_logging, ):
        args = mock.Mock()
        args.port = 1234
        args.config = 'conf.conf'

        m_parse_args.return_value = args
        m_try_to_parse_config_file.return_value = {'a': 'b'}

        val1, val2 = prepare_app_configuration(['qwe'])

        self.assertEqual(val1, {'a': 'b'})
        self.assertEqual(val2, 1234)
        m_parse_args.assert_called_with(['qwe'])
        m_try_to_configure_logging.assert_called_once_with('conf.conf')
        m_try_to_parse_config_file.assert_called_once_with('conf.conf')

    @mock.patch('hpcpm.api.main.create_parser')
    @mock.patch('hpcpm.api.main.add_arguments_to_parser')
    def test_parse_args(self, m_add_arguments_to_parser, m_create_parser):
        parser = mock.MagicMock()
        parser.parse_args.return_value = 'args'
        m_create_parser.return_value = parser

        val = parse_args('a')

        self.assertEqual(val, 'args')
        m_add_arguments_to_parser.assert_called_once_with(parser)
        parser.parse_args.assert_called_once_with('a')

    @mock.patch('argparse.ArgumentParser')
    def test_create_parser(self, m_ArgumentParser):
        m_ArgumentParser.return_value = 'parser'

        val = create_parser()

        self.assertEqual(val, 'parser')
        m_ArgumentParser.assert_called_once_with(description='HPC Power Management - API')

    def test_add_arguments_to_parser(self):
        parser = mock.MagicMock()

        add_arguments_to_parser(parser)

        self.assertEqual(parser.add_argument.call_count, 2)
        parser.add_argument.assert_any_call('-c', '--config',
                                            action='store',
                                            default='api.conf.ini',
                                            help='path to a config file')
        parser.add_argument.assert_called_with('-p', '--port',
                                               action='store',
                                               type=int,
                                               default=8081,
                                               help='port on which API will be exposed')

    @mock.patch('hpcpm.api.main.configure_logging')
    def test_try_to_configure_logging_no_exception(self, m_configure_logging):
        try_to_configure_logging('conf.conf')

        m_configure_logging.assert_called_once_with('conf.conf')

    @mock.patch('hpcpm.api.main.configure_logging')
    @mock.patch('hpcpm.api.main.handle_parsing_error')
    def test_try_to_configure_logging_ParsingError(self, m_handle_parsing_error, m_configure_logging):
        m_configure_logging.side_effect = configparser.ParsingError(source='abcd')

        try_to_configure_logging('conf.conf')

        m_handle_parsing_error.called_once_with()

    @mock.patch('hpcpm.api.main.configure_logging')
    def test_try_to_configure_logging_exception(self, m_configure_logging):
        m_configure_logging.side_effect = Exception()

        self.assertRaises(Exception, try_to_configure_logging, 'conf.conf')

    @mock.patch('logging.config.fileConfig')
    def test_configure_logging(self, m_fileConfig):
        configure_logging('conf.conf')

        m_fileConfig.assert_called_once_with('conf.conf')

    @mock.patch('hpcpm.api.main.parse_config_file')
    def test_try_to_parse_config_file(self, m_parse_config_file):
        m_parse_config_file.return_value = 'config'

        val = try_to_parse_config_file('conf.conf')

        self.assertEqual(val, 'config')
        m_parse_config_file.assert_called_once_with('conf.conf')

    @mock.patch('hpcpm.api.main.parse_config_file')
    @mock.patch('hpcpm.api.main.handle_parsing_error')
    def test_try_to_parse_config_file_ParsingError(self, m_handle_parsing_error, m_parse_config_file):
        m_parse_config_file.side_effect = configparser.ParsingError(source='abcd')

        try_to_parse_config_file('conf.conf')

        m_handle_parsing_error.called_once_with()

    @mock.patch('hpcpm.api.main.parse_config_file')
    def test_try_to_parse_config_file_exception(self, m_parse_config_file):
        m_parse_config_file.side_effect = Exception()

        self.assertRaises(Exception, try_to_parse_config_file, 'conf.conf')

    @mock.patch('configparser.ConfigParser')
    def test_parse_config_file(self, m_ConfigParser):
        config = mock.MagicMock(return_value='config')
        m_ConfigParser.return_value = config

        parse_config_file('conf.conf')

        m_ConfigParser.assert_called_once_with()
        config.read.assert_called_once_with('conf.conf')

    @mock.patch('sys.exit')
    def test_handle_parsing_error(self, m_exit):
        handle_parsing_error()

        m_exit.assert_called_once_with(-1)

    @mock.patch('hpcpm.api.app.initialize')
    @mock.patch('hpcpm.api.app.run')
    def test_run_api(self, m_run, m_initialize):
        run_api('conf', 1234)

        m_initialize.assert_called_once_with('conf')
        m_run.assert_called_once_with(1234)
