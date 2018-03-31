#include "gmock/gmock.h"
#include "geometricKernelConfig.hpp"
#include "contour.hpp"
#include <algorithm>
#include  <stdexcept>
#include <valarray>
#include <armadillo>

using namespace PanosContour;
using namespace testing;
using namespace arma;

namespace PanosContourTest
{


    TEST(points2MeshSurfaceFunction,throwsWhenInputPointsAreLessThanThree)
    {
      vec::fixed<3> p1{0,1,2},p2{3,2,4};
      std::vector<vec::fixed<3>> points{p1,p2};

      ASSERT_THROW(points2MeshSurface(points.begin(),points.end()),std::length_error);


    }


    class APyramidContour : public Test
    {
     public:
      const double pyramid_height=5;
      const double height=pyramid_height/2;

      std::vector<vec::fixed<3>> points{
          {0,0,0},
          {0,1,0},
          {1,0,0},
          {1,1,0},
          {0.5,0.5,pyramid_height}
      };
      //SurfaceContour sc;
      void SetUp() override
      {


      };

    };




    TEST_F(APyramidContour, MeshReturnedIsValid)
    {
      auto dt=triangulate_terrain(points.begin(),points.end());


      auto m= points2MeshSurface(points.begin(), points.end());

      auto mesh_is_valid =m.is_valid();
      ASSERT_TRUE(mesh_is_valid);
      auto number_of_facets = m.number_of_faces();
      ASSERT_EQ(number_of_facets,4);
    }






    TEST_F(APyramidContour,FullPyramidCrossectionIsClosedPolygonWithFourSides)
    {

      auto sc2=SurfaceContour::builder().build(points.begin(),points.end());

      auto contours=sc2->iso_lines_at_height(height);

      ASSERT_EQ((contours.size()),1);
      auto contour_line =contours.front();
      ASSERT_EQ((contour_line.size()),5);
      auto line_is_closed=(contour_line.front()==contour_line.back());
      ASSERT_TRUE(line_is_closed);


    }

    TEST_F(APyramidContour,DoublePyramidCrossectionIsTwoPolygons)
    {


      //append a 2nd pyramidal surface at points vector


      auto translate_point = [&](const vec::fixed<3> p){return p+ vec::fixed<3>{5,0,0};};

      decltype(points) translated_points;

      std::transform(points.begin(),points.end(),std::back_inserter(translated_points),translate_point);

      //append translated_points to points.
      //note: skipping this step by instructing std::transform to insert the results at the end of points looks and probably
      //is dangerous, because of iterator invalidation due to reallocation. Better be safe than strive for optimum performance
      //when testing.
      std::copy(translated_points.begin(),translated_points.end(),std::back_inserter(points));

      std::cout<<"points \n";
      for (const auto p: translated_points)
        {
          for (const auto pp: p)
            std::cout << pp << ' ';
          std::cout << '\n';
        }


      auto sc2=SurfaceContour::builder().build(points.begin(),points.end());

      auto contours=sc2->iso_lines_at_height(height);

      ASSERT_EQ((contours.size()),2);

      {
        auto first_contour_line = contours.front();
        ASSERT_EQ((first_contour_line.size()), 5);
        auto first_line_is_closed = (first_contour_line.front() == first_contour_line.back());
        ASSERT_TRUE(first_line_is_closed);
      }

      {
        auto second_contour_line = contours.back();
        ASSERT_EQ((second_contour_line.size()), 5);
        auto second_line_is_closed = (second_contour_line.front() == second_contour_line.back());
        ASSERT_TRUE(second_line_is_closed);
      }
    }


}




int main (int argc, char **argv)
{

  // The following line must be executed to initialize Google Mock
  // (and Google Test) before running the tests.
  ::testing::InitGoogleMock (&argc, argv);

  return RUN_ALL_TESTS ();
}
