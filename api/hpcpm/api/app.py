import time
import datetime
import numpy as np
import matplotlib.pyplot as plt
from flask import Flask, request, send_file, abort
from tornado.wsgi import WSGIContainer
from tornado.httpserver import HTTPServer
from tornado.ioloop import IOLoop

from hpcpm.api import log
from hpcpm.api.helpers.database import database
from hpcpm.api.resources.ApiSpec import ApiSpec
from hpcpm.api.helpers.utils import abort_when_node_not_found, abort_when_device_not_found
flask_app = Flask(__name__)  # pylint: disable=invalid-name
# pylint: disable=invalid-name
# pylint: disable=too-many-locals


def initialize(config):
    database.configure(config['database'])
    log.info(database)  # to avoid 'unused variable' warning :D
    api_spec = ApiSpec(config['host'])
    flask_app.register_blueprint(api_spec.blueprint, url_prefix='/api/hpcpm')


def run(port):
    http_server = HTTPServer(WSGIContainer(flask_app))
    http_server.bind(port, '0.0.0.0')
    log.debug('HTTP server starting')
    http_server.start(5)
    IOLoop.instance().start()


@flask_app.before_request
def start_request(*_):
    request.real_ip = request.headers.get('X-Real-Ip', request.remote_addr)
    request.start_time = time.time()

    log.info('REQUEST STARTED: %s %s %s', request.real_ip, request.method, request.url)


@flask_app.after_request
def end_request(response_class):
    response_class.headers.add('Access-Control-Allow-Origin', '*')
    response_class.headers.add('Access-Control-Allow-Methods', 'POST, GET, OPTIONS, PUT, DELETE')
    response_class.headers.add('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content-Type, Accept')
    request.end_time = time.time()
    request_processing_time = '{0:.6f}'.format(request.end_time - request.start_time)

    log.info('REQUEST FINISHED (took %s seconds): %s %s %s', request_processing_time, request.real_ip,
             request.method, request.url)
    return response_class


@flask_app.route('/nodes/computation_node/<string:name>/<string:device_id>/statistics_data_plot')
def statistics_data_plot(name, device_id):
    computation_node = abort_when_node_not_found(name)
    abort_when_device_not_found(device_id, computation_node)
    result = database.get_stats_data(name, device_id)
    if not result:
        log.info('No stats for device %s:%s', name, device_id)
        abort(404)
    res_list = sorted([(datetime.datetime.strptime(k, '%Y-%m-%dT%H:%M'), v) for k, v in result.items()])
    list_to_return = [{k.strftime('%Y-%m-%dT%H:%M'): v} for k, v in res_list]
    log.info('Successfully get device %s:%s stats data info: %s', name, device_id, list_to_return)

    values = []
    labels = []
    for element in list_to_return:
        [val] = element.values()
        [lab] = element.keys()
        values.append(val)
        labels.append(lab)

    ind = np.arange(len(list_to_return))  # the x locations for the groups
    width = 0.2  # the width of the bars

    fig, usagePlot = plt.subplots()
    rects1 = usagePlot.bar((ind + width) / 2, values, width, color='r')

    # add some text for labels, title and axes ticks
    usagePlot.set_ylabel('Power usage [W]')
    usagePlot.set_title('Power usage for device')
    usagePlot.set_xticks((ind + width) / 2)
    usagePlot.set_xticklabels(labels)

    # ax.legend(rects1[0], 'Device 1')

    def autolabel(rects):
        # attach some text labels
        for rect in rects:
            height = rect.get_height()
            usagePlot.text(rect.get_x() + rect.get_width() / 2., 1.02 * height,
                           '%d' % int(height),
                           ha='center', va='bottom')

    autolabel(rects1)

    # plt.show()
    fig.savefig('/tmp/plot.png')  # save the figure to file
    plt.close(fig)  # close the figure
    return send_file('/tmp/plot.png')
