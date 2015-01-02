void write_to_array(
    std::ifstream &infile,
    std::array<std::array<unsigned char, 4>, 4> &state
);

void write_to_file(
    std::array<std::array<unsigned char, 4>, 4> state,
    std::ofstream &outfile
);

void open_file(
    std::ifstream &infile,
    std::ofstream &outfile
);
