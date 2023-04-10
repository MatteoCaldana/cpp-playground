#include <array>

constexpr std::array<std::array<uint,9>,27>  
                     _group{{{0,1,2,3,4,5,6,7,8},
                             {9,10,11,12,13,14,15,16,17},
                             {18,19,20,21,22,23,24,25,26},
                             {27,28,29,30,31,32,33,34,35},
                             {36,37,38,39,40,41,42,43,44},
                             {45,46,47,48,49,50,51,52,53},
                             {54,55,56,57,58,59,60,61,62},
                             {63,64,65,66,67,68,69,70,71},
                             {72,73,74,75,76,77,78,79,80},
                             {0,9,18,27,36,45,54,63,72},
                             {1,10,19,28,37,46,55,64,73},
                             {2,11,20,29,38,47,56,65,74},
                             {3,12,21,30,39,48,57,66,75},
                             {4,13,22,31,40,49,58,67,76},
                             {5,14,23,32,41,50,59,68,77},
                             {6,15,24,33,42,51,60,69,78},
                             {7,16,25,34,43,52,61,70,79},
                             {8,17,26,35,44,53,62,71,80},
                             {0,1,2,9,10,11,18,19,20},
                             {3,4,5,12,13,14,21,22,23},
                             {6,7,8,15,16,17,24,25,26},
                             {27,28,29,36,37,38,45,46,47},
                             {30,31,32,39,40,41,48,49,50},
                             {33,34,35,42,43,44,51,52,53},
                             {54,55,56,63,64,65,72,73,74},
                             {57,58,59,66,67,68,75,76,77},
                             {60,61,62,69,70,71,78,79,80}}};

constexpr std::array<std::array<uint,24>,81> 
                     _neighbors{{{1,2,3,4,5,6,7,8,9,18,27,36,45,54,63,72,1,2,9,10,11,18,19,20},
                                 {0,2,3,4,5,6,7,8,10,19,28,37,46,55,64,73,0,2,9,10,11,18,19,20},
                                 {0,1,3,4,5,6,7,8,11,20,29,38,47,56,65,74,0,1,9,10,11,18,19,20},
                                 {0,1,2,4,5,6,7,8,12,21,30,39,48,57,66,75,4,5,12,13,14,21,22,23},
                                 {0,1,2,3,5,6,7,8,13,22,31,40,49,58,67,76,3,5,12,13,14,21,22,23},
                                 {0,1,2,3,4,6,7,8,14,23,32,41,50,59,68,77,3,4,12,13,14,21,22,23},
                                 {0,1,2,3,4,5,7,8,15,24,33,42,51,60,69,78,7,8,15,16,17,24,25,26},
                                 {0,1,2,3,4,5,6,8,16,25,34,43,52,61,70,79,6,8,15,16,17,24,25,26},
                                 {0,1,2,3,4,5,6,7,17,26,35,44,53,62,71,80,6,7,15,16,17,24,25,26},
                                 {10,11,12,13,14,15,16,17,0,18,27,36,45,54,63,72,0,1,2,10,11,18,19,20},
                                 {9,11,12,13,14,15,16,17,1,19,28,37,46,55,64,73,0,1,2,9,11,18,19,20},
                                 {9,10,12,13,14,15,16,17,2,20,29,38,47,56,65,74,0,1,2,9,10,18,19,20},
                                 {9,10,11,13,14,15,16,17,3,21,30,39,48,57,66,75,3,4,5,13,14,21,22,23},
                                 {9,10,11,12,14,15,16,17,4,22,31,40,49,58,67,76,3,4,5,12,14,21,22,23},
                                 {9,10,11,12,13,15,16,17,5,23,32,41,50,59,68,77,3,4,5,12,13,21,22,23},
                                 {9,10,11,12,13,14,16,17,6,24,33,42,51,60,69,78,6,7,8,16,17,24,25,26},
                                 {9,10,11,12,13,14,15,17,7,25,34,43,52,61,70,79,6,7,8,15,17,24,25,26},
                                 {9,10,11,12,13,14,15,16,8,26,35,44,53,62,71,80,6,7,8,15,16,24,25,26},
                                 {19,20,21,22,23,24,25,26,0,9,27,36,45,54,63,72,0,1,2,9,10,11,19,20},
                                 {18,20,21,22,23,24,25,26,1,10,28,37,46,55,64,73,0,1,2,9,10,11,18,20},
                                 {18,19,21,22,23,24,25,26,2,11,29,38,47,56,65,74,0,1,2,9,10,11,18,19},
                                 {18,19,20,22,23,24,25,26,3,12,30,39,48,57,66,75,3,4,5,12,13,14,22,23},
                                 {18,19,20,21,23,24,25,26,4,13,31,40,49,58,67,76,3,4,5,12,13,14,21,23},
                                 {18,19,20,21,22,24,25,26,5,14,32,41,50,59,68,77,3,4,5,12,13,14,21,22},
                                 {18,19,20,21,22,23,25,26,6,15,33,42,51,60,69,78,6,7,8,15,16,17,25,26},
                                 {18,19,20,21,22,23,24,26,7,16,34,43,52,61,70,79,6,7,8,15,16,17,24,26},
                                 {18,19,20,21,22,23,24,25,8,17,35,44,53,62,71,80,6,7,8,15,16,17,24,25},
                                 {28,29,30,31,32,33,34,35,0,9,18,36,45,54,63,72,28,29,36,37,38,45,46,47},
                                 {27,29,30,31,32,33,34,35,1,10,19,37,46,55,64,73,27,29,36,37,38,45,46,47},
                                 {27,28,30,31,32,33,34,35,2,11,20,38,47,56,65,74,27,28,36,37,38,45,46,47},
                                 {27,28,29,31,32,33,34,35,3,12,21,39,48,57,66,75,31,32,39,40,41,48,49,50},
                                 {27,28,29,30,32,33,34,35,4,13,22,40,49,58,67,76,30,32,39,40,41,48,49,50},
                                 {27,28,29,30,31,33,34,35,5,14,23,41,50,59,68,77,30,31,39,40,41,48,49,50},
                                 {27,28,29,30,31,32,34,35,6,15,24,42,51,60,69,78,34,35,42,43,44,51,52,53},
                                 {27,28,29,30,31,32,33,35,7,16,25,43,52,61,70,79,33,35,42,43,44,51,52,53},
                                 {27,28,29,30,31,32,33,34,8,17,26,44,53,62,71,80,33,34,42,43,44,51,52,53},
                                 {37,38,39,40,41,42,43,44,0,9,18,27,45,54,63,72,27,28,29,37,38,45,46,47},
                                 {36,38,39,40,41,42,43,44,1,10,19,28,46,55,64,73,27,28,29,36,38,45,46,47},
                                 {36,37,39,40,41,42,43,44,2,11,20,29,47,56,65,74,27,28,29,36,37,45,46,47},
                                 {36,37,38,40,41,42,43,44,3,12,21,30,48,57,66,75,30,31,32,40,41,48,49,50},
                                 {36,37,38,39,41,42,43,44,4,13,22,31,49,58,67,76,30,31,32,39,41,48,49,50},
                                 {36,37,38,39,40,42,43,44,5,14,23,32,50,59,68,77,30,31,32,39,40,48,49,50},
                                 {36,37,38,39,40,41,43,44,6,15,24,33,51,60,69,78,33,34,35,43,44,51,52,53},
                                 {36,37,38,39,40,41,42,44,7,16,25,34,52,61,70,79,33,34,35,42,44,51,52,53},
                                 {36,37,38,39,40,41,42,43,8,17,26,35,53,62,71,80,33,34,35,42,43,51,52,53},
                                 {46,47,48,49,50,51,52,53,0,9,18,27,36,54,63,72,27,28,29,36,37,38,46,47},
                                 {45,47,48,49,50,51,52,53,1,10,19,28,37,55,64,73,27,28,29,36,37,38,45,47},
                                 {45,46,48,49,50,51,52,53,2,11,20,29,38,56,65,74,27,28,29,36,37,38,45,46},
                                 {45,46,47,49,50,51,52,53,3,12,21,30,39,57,66,75,30,31,32,39,40,41,49,50},
                                 {45,46,47,48,50,51,52,53,4,13,22,31,40,58,67,76,30,31,32,39,40,41,48,50},
                                 {45,46,47,48,49,51,52,53,5,14,23,32,41,59,68,77,30,31,32,39,40,41,48,49},
                                 {45,46,47,48,49,50,52,53,6,15,24,33,42,60,69,78,33,34,35,42,43,44,52,53},
                                 {45,46,47,48,49,50,51,53,7,16,25,34,43,61,70,79,33,34,35,42,43,44,51,53},
                                 {45,46,47,48,49,50,51,52,8,17,26,35,44,62,71,80,33,34,35,42,43,44,51,52},
                                 {55,56,57,58,59,60,61,62,0,9,18,27,36,45,63,72,55,56,63,64,65,72,73,74},
                                 {54,56,57,58,59,60,61,62,1,10,19,28,37,46,64,73,54,56,63,64,65,72,73,74},
                                 {54,55,57,58,59,60,61,62,2,11,20,29,38,47,65,74,54,55,63,64,65,72,73,74},
                                 {54,55,56,58,59,60,61,62,3,12,21,30,39,48,66,75,58,59,66,67,68,75,76,77},
                                 {54,55,56,57,59,60,61,62,4,13,22,31,40,49,67,76,57,59,66,67,68,75,76,77},
                                 {54,55,56,57,58,60,61,62,5,14,23,32,41,50,68,77,57,58,66,67,68,75,76,77},
                                 {54,55,56,57,58,59,61,62,6,15,24,33,42,51,69,78,61,62,69,70,71,78,79,80},
                                 {54,55,56,57,58,59,60,62,7,16,25,34,43,52,70,79,60,62,69,70,71,78,79,80},
                                 {54,55,56,57,58,59,60,61,8,17,26,35,44,53,71,80,60,61,69,70,71,78,79,80},
                                 {64,65,66,67,68,69,70,71,0,9,18,27,36,45,54,72,54,55,56,64,65,72,73,74},
                                 {63,65,66,67,68,69,70,71,1,10,19,28,37,46,55,73,54,55,56,63,65,72,73,74},
                                 {63,64,66,67,68,69,70,71,2,11,20,29,38,47,56,74,54,55,56,63,64,72,73,74},
                                 {63,64,65,67,68,69,70,71,3,12,21,30,39,48,57,75,57,58,59,67,68,75,76,77},
                                 {63,64,65,66,68,69,70,71,4,13,22,31,40,49,58,76,57,58,59,66,68,75,76,77},
                                 {63,64,65,66,67,69,70,71,5,14,23,32,41,50,59,77,57,58,59,66,67,75,76,77},
                                 {63,64,65,66,67,68,70,71,6,15,24,33,42,51,60,78,60,61,62,70,71,78,79,80},
                                 {63,64,65,66,67,68,69,71,7,16,25,34,43,52,61,79,60,61,62,69,71,78,79,80},
                                 {63,64,65,66,67,68,69,70,8,17,26,35,44,53,62,80,60,61,62,69,70,78,79,80},
                                 {73,74,75,76,77,78,79,80,0,9,18,27,36,45,54,63,54,55,56,63,64,65,73,74},
                                 {72,74,75,76,77,78,79,80,1,10,19,28,37,46,55,64,54,55,56,63,64,65,72,74},
                                 {72,73,75,76,77,78,79,80,2,11,20,29,38,47,56,65,54,55,56,63,64,65,72,73},
                                 {72,73,74,76,77,78,79,80,3,12,21,30,39,48,57,66,57,58,59,66,67,68,76,77},
                                 {72,73,74,75,77,78,79,80,4,13,22,31,40,49,58,67,57,58,59,66,67,68,75,77},
                                 {72,73,74,75,76,78,79,80,5,14,23,32,41,50,59,68,57,58,59,66,67,68,75,76},
                                 {72,73,74,75,76,77,79,80,6,15,24,33,42,51,60,69,60,61,62,69,70,71,79,80},
                                 {72,73,74,75,76,77,78,80,7,16,25,34,43,52,61,70,60,61,62,69,70,71,78,80},
                                 {72,73,74,75,76,77,78,79,8,17,26,35,44,53,62,71,60,61,62,69,70,71,78,79}}};

constexpr std::array<std::array<uint,3>,81> 
                     _groups_of{{{0,9,18},
                                 {0,10,18},
                                 {0,11,18},
                                 {0,12,19},
                                 {0,13,19},
                                 {0,14,19},
                                 {0,15,20},
                                 {0,16,20},
                                 {0,17,20},
                                 {1,9,18},
                                 {1,10,18},
                                 {1,11,18},
                                 {1,12,19},
                                 {1,13,19},
                                 {1,14,19},
                                 {1,15,20},
                                 {1,16,20},
                                 {1,17,20},
                                 {2,9,18},
                                 {2,10,18},
                                 {2,11,18},
                                 {2,12,19},
                                 {2,13,19},
                                 {2,14,19},
                                 {2,15,20},
                                 {2,16,20},
                                 {2,17,20},
                                 {3,9,21},
                                 {3,10,21},
                                 {3,11,21},
                                 {3,12,22},
                                 {3,13,22},
                                 {3,14,22},
                                 {3,15,23},
                                 {3,16,23},
                                 {3,17,23},
                                 {4,9,21},
                                 {4,10,21},
                                 {4,11,21},
                                 {4,12,22},
                                 {4,13,22},
                                 {4,14,22},
                                 {4,15,23},
                                 {4,16,23},
                                 {4,17,23},
                                 {5,9,21},
                                 {5,10,21},
                                 {5,11,21},
                                 {5,12,22},
                                 {5,13,22},
                                 {5,14,22},
                                 {5,15,23},
                                 {5,16,23},
                                 {5,17,23},
                                 {6,9,24},
                                 {6,10,24},
                                 {6,11,24},
                                 {6,12,25},
                                 {6,13,25},
                                 {6,14,25},
                                 {6,15,26},
                                 {6,16,26},
                                 {6,17,26},
                                 {7,9,24},
                                 {7,10,24},
                                 {7,11,24},
                                 {7,12,25},
                                 {7,13,25},
                                 {7,14,25},
                                 {7,15,26},
                                 {7,16,26},
                                 {7,17,26},
                                 {8,9,24},
                                 {8,10,24},
                                 {8,11,24},
                                 {8,12,25},
                                 {8,13,25},
                                 {8,14,25},
                                 {8,15,26},
                                 {8,16,26},
                                 {8,17,26}}};