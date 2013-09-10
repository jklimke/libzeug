
#include <iostream>
#include <QApplication>
#include <QTreeView>
#include <QWidget>
#include <QDebug>
#include <propertyzeug/PropertyGroup.h>
#include <propertyguizeug/PropertyItemModel.h>
#include <propertyguizeug/PropertyDelegate.h>

using namespace propertyzeug;
using namespace propertyguizeug;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    PropertyGroup settings("Settings");
    PropertyGroup constraints("Constraints");
    constraints.addProperty<std::vector<int>>("Position", std::vector<int>({ 100, 200 }));
    settings.addProperty(&constraints);
    constraints.addProperty("Height", 3);
    constraints.property<int>("Height")->setMinimum(0);
    constraints.property<int>("Height")->setMaximum(100);
    constraints.addProperty("Width", 12);
    
    QTreeView treeView;
    treeView.setAlternatingRowColors(true);
    PropertyItemModel model(&settings);
    PropertyDelegate delegate;
    treeView.setModel(&model);
    treeView.setItemDelegateForColumn(1, &delegate);
    treeView.show();
    treeView.expandAll();
    
    return a.exec();
}