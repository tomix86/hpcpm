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
        nodeRow: function (nodeName) {
            return {
                get: function () { //unfortunately returns a promise
                    return thisPage.nodesTable.get().filter(function (row) {
                        return row.all(by.tagName('td')).get(0).getText().then(function (txt) {
                            return txt === nodeName;
                        });
                    }).then(function (elem) {
                        return elem[0];
                    });
                },
                nameField: function () {
                    return thisPage.nodesTable.nodeRow(nodeName).field(0)
                },
                field: function (number) {
                    return {
                        get: function () {
                            return thisPage.nodesTable.nodeRow(nodeName).get().then(function (row) {
                                return row.all(by.tagName('td')).get(number)
                            })
                        },
                        link: {
                            click: function () {
                                thisPage.nodesTable.nodeRow(nodeName).field(number).get().then(function (field) {
                                    field.element(by.tagName('a')).click()
                                });
                            }
                        }
                    }
                }
            }
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
        },
        addButton: {
            click: function () {
                thisPage.modal.get().element(by.partialButtonText('Add')).click()
            }
        }
    };

    this.toasts = {
        divs: {
            get: function () {
                return element(by.id('toast-container')).all(by.tagName('div'))
            },
            count: function () {
                return thisPage.toasts.divs.get().count()
            }
        },
        top: {
            title: {
                get: function () {
                    return thisPage.toasts.divs.get().get(1).getText()
                }
            },
            content: {
                get: function () {
                    return thisPage.toasts.divs.get().get(2).getText()
                }
            }
        },
        any: function () {
            return thisPage.toasts.divs.count().then(function (count) {
                return count > 0;
            });
        },
        empty: function () {
            return thisPage.toasts.divs.count().then(function (count) {
                return count === 0;
            });
        }
    };

    this.pageHeader = {
        get: function () {
            return element(by.css('[class="page-header"]')).getText()
        }
    }
};


module.exports = HPCPMPage;
