#ifndef LAB3_CLASSES_HPP
#define LAB3_CLASSES_HPP

#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <cfloat>

using namespace std;

vector<string> split(const string &s, char delimiter, bool space) {
    stringstream ss(s);
    string item;
    vector<string> elements;

    while (getline(ss, item, delimiter)) {
        elements.push_back(item);
        if (space) ss.get();
    }

    return elements;
}

vector<double> split(const string &s, char delimiter) {
    stringstream ss(s);
    string item;
    vector<double> elements;

    while (getline(ss, item, delimiter)) {
        elements.push_back(stod(item));
    }

    return elements;
}

class Stop {
public:
    explicit Stop(double latitude = 0, double longitude = 0) : latitude(latitude), longitude(longitude) {}

    friend ostream& operator<<(ostream& stream, const Stop& route);

    double getLat() const { return latitude / 57.29577951; }

    double getLon() const { return longitude / 57.29577951; }

private:
    double latitude, longitude;
};

bool operator==(const Stop& lhs, const Stop& rhs) {
    return (lhs.getLat() == rhs.getLat()) && (lhs.getLon() == rhs.getLon());
}

bool operator!=(const Stop& lhs, const Stop& rhs) {
    return !(lhs == rhs);
}

ostream& operator<<(ostream& stream, const Stop& route) {
    stream << route.latitude << ", " << route.longitude;
    return stream;
}

double getDistance(const Stop& p1, const Stop& p2) {
    return 6372795 * 2 * asin(sqrt(pow(sin((p2.getLat() - p1.getLat()) / 2), 2) + cos(p1.getLat()) * cos(p2.getLat()) * pow(sin((p2.getLon() - p1.getLon()) / 2), 2)));
}

class Route {
public:
    Route() : length(0) {

    }

    void addStop(Stop& stop) {
        stops.push_back(stop);
    }

    int numberOfStops() const {
        return stops.size();
    }

    double getLength() {
        if (length == 0)
            calculateLength();
        return length;
    }

    void calculateLength() {
        auto maxDist = DBL_MIN;
        int pos = 0;

        // Trying to find first stop
        for (int i = 0; i < stops.size(); ++i) {
            for (int j = i + 1; j < stops.size(); ++j) {
                double dist = getDistance(stops[i], stops[j]);
                if (dist > maxDist) {
                    maxDist = dist;
                    pos = i;
                }
            }
        }

        vector<Stop> sortedRoute;
        sortedRoute.push_back(stops[pos]);
        stops.erase(stops.begin() + pos);

        while (!stops.empty()) {
            auto minDist = DBL_MAX;
            pos = 0;
            for (int i = 0; i < stops.size(); ++i) {
                double dist = getDistance(sortedRoute[sortedRoute.size() - 1], stops[i]);
                if (dist < minDist) {
                    minDist = dist;
                    pos = i;
                }
            }
            if (minDist < 10000) {
                length += minDist;
            }
            sortedRoute.push_back(stops[pos]);
            stops.erase(stops.begin() + pos);
        }

        stops.clear();
        stops = sortedRoute;

    }

    friend ostream& operator<<(ostream& stream, const Route& route);

private:
    vector<Stop> stops;
    double length;
};

ostream& operator<<(ostream& stream, const Route& route) {
    for (Stop stop : route.stops) {
        stream << stop << " ";
    }
    return stream;
}

void printMap(map<string, Route>& map, const string& fileName) {
    ofstream file;
    file.open(fileName, ios::out);
    for (auto & item : map) {
        file << item.first << " " << item.second.getLength() << " " << item.second << endl;
    }
    file.close();
}

void findMaxStops(const map<string, Route>& vehicleType, const string& vehicleName) {
    int max = 0;
    string routeName;
    for (auto & route : vehicleType) {
        if (route.second.numberOfStops() > max) {
            max = route.second.numberOfStops();
            routeName = route.first;
        }
    }

    cout << vehicleName << " №" << routeName << " " << max << " остановок"<< endl;
}

void findMaxLength(map<string, Route>& vehicleType, const string& vehicleName) {
    double max = 0;
    string routeName;
    for (auto & route : vehicleType) {
        if (route.second.getLength() > max) {
            max = route.second.getLength();
            routeName = route.first;
        }
    }

    cout << vehicleName << " №" << routeName << " " << max << " метров"<< endl;
}

void findMaxStopsStreet(map<string, int>& streets) {
    multimap<int, string> streetsSorted;

    for (auto& street : streets) {
        streetsSorted.insert({street.second, street.first});
    }

    cout << "Макимум остановок на улице:" << endl;
    cout << (--streetsSorted.end())->second << " " << (--streetsSorted.end())->first << endl;

    ofstream out;
    out.open("_streets.txt", ios::out);
    for (auto it = --streetsSorted.end(); it != streetsSorted.begin(); --it) {
        out << it->second << " " << it->first << endl;
    }

    out.close();
}

#endif //LAB3_CLASSES_HPP
