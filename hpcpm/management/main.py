"""Module for entry point of Management application"""
import argparse
import configparser
import logging
import logging.config
import sys
from hpcpm.management.app import App
from hpcpm.management import log


def main():
    """Entry point of application"""
    config = prepare_app_configuration(sys.argv[1:])
    run_app(config)
    return 0


def prepare_app_configuration(argv):
    args = parse_args(argv)
    try_to_configure_logging(args.config)
    config = try_to_parse_config_file(args.config)
    return config


def parse_args(argv):
    parser = create_parser()
    log.debug("parser created")

    add_arguments_to_parser(parser)
    args = parser.parse_args(argv)
    log.debug("arguments parsed")

    return args


def create_parser():
    log.debug("creating argument parser")
    return argparse.ArgumentParser(description="HPC Power Management - Management")


def add_arguments_to_parser(parser):
    parser.add_argument("-c", "--config",
                        action="store",
                        default="management.conf.ini",
                        help="path to a config file")


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


def run_app(config):
    application = App(config)
    application.run()

if __name__ == '__main__':
    sys.exit(main())
