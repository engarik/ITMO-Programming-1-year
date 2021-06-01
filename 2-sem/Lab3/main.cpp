#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>

#include "pugixml.hpp"
#include "classes.hpp"

using namespace pugi;
using namespace std;

int main() {
    // Some out configs
    cout.precision(0);
    cout << fixed;
    system("CHCP 65001");

    // Loading xml document
    xml_document doc;
    doc.load_file("data.xml");
    xml_node dataset = doc.child("dataset");

    // Structures to keep data
    map<string, int> streets;
    map<string, Route> bus;
    map<string, Route> troll;
    map<string, Route> tram;

    // Scanning xml
    for (xml_node station : dataset.children()) {
        string vehicleType = station.child("type_of_vehicle").child_value(), routeString = station.child("routes").child_value();
        vector<string> routes;

        if (routeString.find('.') != string::npos) {
            routes = split(routeString, '.', false);
        } else {
            routes = split(routeString, ',', false);
        }

        vector<double> pos = split(station.child("coordinates").child_value(), ',');
        Stop stop(pos[0], pos[1]); // Init a stop

        if (vehicleType == "Автобус") {
            for (const string& routeNumber : routes) {
                bus[routeNumber].addStop(stop);
            }
        } else if (vehicleType == "Троллейбус") {
            for (const string& routeNumber : routes) {
                troll[routeNumber].addStop(stop);
            }
        } else if (vehicleType == "Трамвай") {
            for (const string& routeNumber : routes) {
                tram[routeNumber].addStop(stop);
            }
        }

        vector<string> res = split(station.child("location").child_value(), ',', true);
        for (const string& street : res) {
            streets[street] += 1;
        }
    }

    // Output

    cout << "Максимум остановок:" << endl;
    findMaxStops(bus, "Автобус");
    findMaxStops(troll, "Троллейбус");
    findMaxStops(tram, "Трамвай");
    cout << endl;

    cout << "Самый длинный маршрут:" << endl;
    findMaxLength(bus, "Автобус");
    findMaxLength(troll, "Троллейбус");
    findMaxLength(tram, "Трамвай");
    cout << endl;

    printMap(bus, "_bus.txt");
    printMap(troll, "_troll.txt");
    printMap(tram, "_tram.txt");


    findMaxStopsStreet(streets);

    return 0;
}
