"""Module for entry point of API application"""
import argparse
import configparser
import logging
import logging.config
import sys
from hpcpm.api import app
from hpcpm.api import log


def main():
    """Entry point of application"""
    config, port = prepare_app_configuration(sys.argv[1:])
    run_api(config, port)
    return 0


def prepare_app_configuration(argv):
    args = parse_args(argv)
    try_to_configure_logging(args.config)
    config = try_to_parse_config_file(args.config)
    return config, args.port


def parse_args(argv):
    parser = create_parser()
    log.debug("parser created")

    add_arguments_to_parser(parser)
    args = parser.parse_args(argv)
    log.debug("arguments parsed")

    return args


def create_parser():
    log.debug("creating argument parser")
    return argparse.ArgumentParser(description="HPC Power Management - API")


def add_arguments_to_parser(parser):
    parser.add_argument("-c", "--config",
                        action="store",
                        default="api.conf.ini",
                        help="path to a config file")

    parser.add_argument("-p", "--port",
                        action="store",
                        type=int,
                        default=8080,
                        help="port on which API will be exposed")


def try_to_configure_logging(config_file_path):
    try:
        configure_logging(config_file_path)
    except configparser.ParsingError:
        handle_parsing_error()


def configure_logging(config_file_path):
    logging.config.fileConfig(config_file_path)
    log.debug("Logging configuration loaded from %s", config_file_path)


def try_to_parse_config_file(config_file_path):
    try:
        return parse_config_file(config_file_path)
    except configparser.ParsingError:
        handle_parsing_error()


def parse_config_file(config_file_path):
    config = configparser.ConfigParser()
    config.read(config_file_path)
    log.debug("Application configuration loaded from %s", config_file_path)
    return config


def handle_parsing_error():
    log.exception("Config file could not be parsed")
    sys.exit(-1)


def run_api(config, port):
    app.initialize(config)
    app.run(port)

if __name__ == '__main__':
    sys.exit(main())
