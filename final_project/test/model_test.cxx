#include "model.hxx"
#include <catch.hxx>
#include "board.hxx"

////BOARD TEST FUNCTIONS

TEST_CASE("board: init has correct states ")
{
    Board b({5,5},0);
    int count=0;
    for(Board::Position pos: b.get_pos_mines())
    {
        CHECK(b.compare_pos(pos,{0,0}));
        count++;
    }
    CHECK(count==b.get_num_mines());
    for(Board::Position pos: b.all_positions())
    {
        Board::pos_states state = b.get_pos_data(pos);
        CHECK_FALSE(state.mine);
        CHECK_FALSE(state.unveiled);
        CHECK_FALSE(state.flagged);
        CHECK(state.num_of_mines ==0);
    }
}

TEST_CASE("board: has correct number of mines, even when manually set")
{
    Board b({5,4},10);
    CHECK(static_cast<int>(b.get_pos_mines().size())==10);
    if(b.set_mines({1,4,5,6,7,
                       9,10,12,14,18}))
    {
        std::vector<Board::Position> v2 = b.get_pos_mines();
        CHECK(static_cast<int>(v2.size())==10);
        std::vector<Board::Position> v1 ={{1,0}, {4,0}, {0,1},
                                         {1,1}, {2,1},{4,1},
                                         {0,2}, {2,2}, {4,2},
                                         {3,3}};
        for(int i = 0; i<10; i++)
        {
            CHECK(b.compare_pos(v1[i],v2[i]));
        }
    }
}

TEST_CASE("board: int_to_pos")
{
    Board b({3,3},0);
    CHECK(b.compare_pos(b.int_to_pos(0),{0,0}));
    CHECK(b.compare_pos(b.int_to_pos(3),{0,1}));
    CHECK(b.compare_pos(b.int_to_pos(7),{1,2}));

    Board b1({4,3},0);

    CHECK(b1.compare_pos(b1.int_to_pos(1),{1,0}));
    CHECK(b1.compare_pos(b1.int_to_pos(3),{3,0}));

    Board::Position pos = b1.int_to_pos(6);
    CHECK(b1.compare_pos(pos,{2,1}));
}

TEST_CASE("board: pos_to_int")
{
    Board b({4,3},0);
    CHECK(b.pos_to_int({0,0})==0);
    CHECK(b.pos_to_int({1,2})==9);
    CHECK(b.pos_to_int({2,1})==6);
}

TEST_CASE("board: random mine placement")
{
    //Same conditions
    Board b1({5,4},10);
    Board b2({5,4},10);
    //same # of mines
    CHECK(b1.get_pos_mines().size()==b2.get_pos_mines().size());
    //Different placement
    CHECK_FALSE(b2.compare_boards(b1));
}

TEST_CASE("board: correct number of surrounding mines")
{
    //Init: no mines -> all number tiles should be 0
    Board b1({2, 2}, 0);
    CHECK(b1.get_pos_data({0, 0}).num_of_mines == 0);

    //Init: 1 randomly placed mine, relocated to {1,1}
    //all number tiles should be 1
    Board b2({3, 3}, 1);
    if(b2.set_mines(std::vector<int>(1, 4)))
    {
        CHECK(b2.compare_pos(b2.get_pos_mines()[0], {1, 1}));
        CHECK(b2.get_pos_data({0, 0}).num_of_mines == 1);
        CHECK(b2.get_pos_data({2, 1}).num_of_mines == 1);
    }
    //Init: 1 randomly placed mine, relocated to {0,0}
    //3 number tiles should be 1: {1,0}, {1,1}, {0,1}
    //5 number tiles should be 0: {0,2},{1,2},{2,2},{2,1},{2,0}
    Board b3({3, 4}, 1);
    if(b3.set_mines(std::vector<int>(1, 0)))
    {
        CHECK(b3.get_pos_data({1, 1}).num_of_mines == 1);
        CHECK(b3.get_pos_data({2, 2}).num_of_mines == 0);
    }
    //Init: 3 randomly placed mines, relocated to {1,0}, {3,0}, {2,1}
    Board b4({4, 3}, 3);
    if(b4.set_mines(std::vector<int> {1, 3, 6})) {
        CHECK(b4.get_pos_data({1, 0}).mine);
        CHECK(b4.get_pos_data({3, 0}).mine);
        CHECK(b4.get_pos_data({2, 1}).mine);
        CHECK(b4.get_pos_data({1, 2}).num_of_mines == 1);
        CHECK(b4.get_pos_data({0, 2}).num_of_mines == 0);
        CHECK(b4.get_pos_data({2, 0}).num_of_mines == 3);
    }
}

TEST_CASE("board: unveiling correct mine tiles")
{
    Board b({3,3},2);
    b.set_mines(std::vector<int>{1,0});
    CHECK(b.get_pos_data({0,0}).mine);
    CHECK(b.get_pos_data({1,0}).mine);
    b.unveil_mines();
    CHECK(b.get_pos_data({0,0}).unveiled);
    CHECK(b.get_pos_data({1,0}).unveiled);

}

////MODEL TEST CASES

TEST_CASE("correct initial states")
{
    Model m(10,10, 0);
    CHECK(m.check_unveiled_({5,4})==false);
    CHECK(m.check_mine_({6,2})==false);
    CHECK(m.check_flagged_({1,9})==false);
}

TEST_CASE("model:set_mines")
{
    Model m(3,3, 1);
    if(m.set_mines({{1,1}}))
    {
        CHECK(m.check_mine_({1,1}));
        CHECK(m.get_board().get_pos_data({1,1}).mine);
        m.unveil_tile({1,1});
        CHECK(m.get_unveiled_tiles_()==1);
    }
}

TEST_CASE("model: check unveil_tiles->mines")
{
    Model m(3,3, 2);
    if(m.set_mines({{0,0},{1,0}}))
     {
        CHECK(m.check_mine_({0,0}));
        CHECK(m.get_board().get_pos_data({1,0}).mine);
        m.unveil_tile({0,0});
        CHECK(m.get_unveiled_tiles_()==2);
        CHECK(m.check_unveiled_({1,0}));
        CHECK(m.is_game_over());
        CHECK_FALSE(m.win_or_loss());
     }
}


TEST_CASE("check unveil_tiles->numbers: 1 mine")
{
    Model m(3,3, 1);
    if(m.set_mines({{2,1}}))
    {
        CHECK(m.check_mine_({2,1}));
        CHECK(m.get_board().get_pos_data({2,1}).mine);
        m.unveil_tile({0,1});
        CHECK(m.check_unveiled_({0,1}));
        CHECK(m.check_unveiled_({0,2}));
        CHECK(m.check_unveiled_({0,0}));
        CHECK(m.get_unveiled_tiles_()==3);
        CHECK_FALSE(m.win_or_loss());
        CHECK_FALSE(m.is_game_over());
        m.unveil_tile({1,0});
        CHECK(m.surrounding_mines_({1,0})==1);
        m.unveil_tile({2,0});
        m.unveil_tile({1,1});
        CHECK(m.surrounding_mines_({1,1})==1);
        m.unveil_tile({1,2});
        m.unveil_tile({2,2});
        CHECK(m.surrounding_mines_({2,2})==1);
        CHECK(m.get_unveiled_tiles_()==8);
        CHECK(m.is_game_over());
        CHECK(m.win_or_loss());
    }
}

TEST_CASE("check unveil_tiles->numbers: 2 mines")
{
    Model m(3,4, 3);
    if(m.set_mines({{0,0},{1,1},{1,2}}))
    {
        CHECK(m.check_mine_({1,1}));
        CHECK(m.check_mine_({0,0}));
        CHECK(m.check_mine_({1,2}));
        m.unveil_tile({0,1});
        CHECK(m.surrounding_mines_({0,1})==3);
        CHECK(m.check_unveiled_({0,1}));
        CHECK(m.get_unveiled_tiles_()==1);
        m.unveil_tile({1,0});
        CHECK(m.surrounding_mines_({1,0})==2);
        m.unveil_tile({2,0});
        m.unveil_tile({2,1});
        m.unveil_tile({0,2});
        m.unveil_tile({2,2});
        m.unveil_tile({0,3});
        m.unveil_tile({1,3});
        m.unveil_tile({2,3});
        CHECK(m.get_unveiled_tiles_()==9);
        CHECK(m.is_game_over());
        CHECK(m.win_or_loss());
    }
}

TEST_CASE("check flag_tiles")
{
    Model m(3,4, 1);
    if(m.set_mines({{1,1}}))
    {
        CHECK_FALSE(m.get_board().get_flagged({2,3}));
        m.flag_tile({2,3});
        CHECK(m.check_flagged_({2,3}));
        m.flag_tile({1,0});
        CHECK(m.check_flagged_({1,0}));
        m.flag_tile({0,2});
        CHECK(m.check_flagged_({0,2}));
        m.unveil_tile({0,2});
        CHECK_FALSE(m.check_unveiled_({0,2}));
        CHECK(m.get_flag_counter()==3);
        m.flag_tile({2,3});
        CHECK_FALSE(m.check_flagged_({2,3}));
        CHECK(m.get_flag_counter()==2);
    }
}

TEST_CASE("check restart")
{
    Model m(3,4, 1);
    Board b1 = m.change_board();
    m.restart();
    Board b2 = m.change_board();
    //checking board is changed
    CHECK_FALSE(b2.compare_boards(b1));
}
