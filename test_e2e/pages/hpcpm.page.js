var HPCPMPage = function () {
    var thisPage = this;
    this.get = function () {
        browser.get('/');
    };
    this.title = {
        get: function () {
            return browser.getTitle();
        }
    };

    this.nodesTable = {
        get: function () {
            return element(by.css('[ng-controller="NodesController"]')).element(by.css('[ng-table="tableParams"]')).all(by.repeater('row in $data'))
        },
        getNodeRow: function (nodeName) {
            return thisPage.nodesTable.get().filter(function (row) {
                return row.all(by.tagName('td')).get(0).getText().then(function (txt) {
                    return txt === nodeName;
                });
            }).then(function (elem) {
                return elem[0];
            });
        }
    };

    this.addNodeButton = {
        click: function () {
            element(by.css('[ng-click="OpenNewNodeModal()"]')).click()
        }
    };

    this.modal = {
        get: function () {
            return element(by.css('[class="modal-dialog"]'))
        },
        title: {
            get: function () {
                return thisPage.modal.get().element(by.css('[class="modal-title"]')).getText()
            }
        },
        nodeName: {
            set: function (name) {
                 thisPage.modal.get().element(by.model('node.name')).sendKeys(name)
            }
        },
        nodeAddress: {
            set: function (address) {
                 thisPage.modal.get().element(by.model('node.address')).sendKeys(address)
            }
        },
        nodePort: {
            set: function (port) {
                 thisPage.modal.get().element(by.model('node.port')).sendKeys(port)
            }
        }

    }

};


module.exports = HPCPMPage;
