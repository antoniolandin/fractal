class ColorMap {
public:
    ColorMap(int step);
    ~ColorMap();
    void map(int numero, int* r, int* g, int* b);
private:
    int* r_map;
    int*  g_map;
    int* b_map;
    int len;
    int step;
};
