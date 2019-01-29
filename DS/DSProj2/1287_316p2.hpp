#ifndef VERTEX_HPP_INCLUDED
#define VERTEX_HPP_INCLUDED

struct Vertex {
    int thisVertex;
    double shortestDist;  // Shortest distance from source vertex
    int prevVertex;       // Previous vertex following the current shortest path

    Vertex():thisVertex(-1),shortestDist(std::numeric_limits<double>::infinity()),prevVertex(-1) {}
    Vertex(double n):thisVertex(n),shortestDist(std::numeric_limits<double>::infinity()),prevVertex(-1) {}
};

struct OnOffSwitch {
    bool theSwitch;
    Vertex vert;
};
#endif // VERTEX_HPP_INCLUDED
