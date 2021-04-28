#include"D2game.cpp"
int main(){
    PlantLifeGame<10> plg;
    std::vector<coordinate> glider;
    glider.push_back({0,2});
    glider.push_back({1,2});
    glider.push_back({2,2});
    glider.push_back({2,1});
    glider.push_back({1,0});
    plg.reset(glider);
    // plg.show();
    plg.run(16,1000);
    return 0;
}
// int main(){
//     OneDimension<16> od({1,2,3,7,4,15,11,22});
//     od.run();
// }