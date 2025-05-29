#include <gtest/gtest.h>

#include "../viewer_model/viewer_model.h"

class ViewerModelTest : public ::testing::Test {};

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST_F(ViewerModelTest, Test_1) {
  s21::ViewerModel model;
  model.translateFigure(translateXPlus, 0.0f);
  EXPECT_EQ(model.getAffineTransform().translateX, 0.5f);
  model.translateFigure(translateXMinus, 0.1f);
  EXPECT_EQ(model.getAffineTransform().translateX, 0.4f);
  model.translateFigure(translateYPlus, 0.1f);
  EXPECT_EQ(model.getAffineTransform().translateY, 0.1f);
  model.translateFigure(translateYMinus, 0.1f);
  EXPECT_EQ(model.getAffineTransform().translateY, 0.0f);
  model.translateFigure(translateZPlus, 0.1f);
  EXPECT_EQ(model.getAffineTransform().translateZ, 0.1f);
  model.translateFigure(translateZMinus, 0.1f);
  EXPECT_EQ(model.getAffineTransform().translateZ, 0.0f);
}

TEST_F(ViewerModelTest, Test_2) {
  s21::ViewerModel model;
  model.rotateAxis(rotateXPlus, 0.0f);
  EXPECT_EQ(model.getAffineTransform().rotateAngleX, 15.0f);
  model.rotateAxis(rotateXMinus, 1.0f);
  EXPECT_EQ(model.getAffineTransform().rotateAngleX, 14.0f);
  model.rotateAxis(rotateYPlus, 1.0f);
  EXPECT_EQ(model.getAffineTransform().rotateAngleY, 1.0f);
  model.rotateAxis(rotateYMinus, 1.0f);
  EXPECT_EQ(model.getAffineTransform().rotateAngleY, 0.0f);
  model.rotateAxis(rotateZPlus, 1.0f);
  EXPECT_EQ(model.getAffineTransform().rotateAngleZ, 1.0f);
  model.rotateAxis(rotateZMinus, 1.0f);
  EXPECT_EQ(model.getAffineTransform().rotateAngleZ, 0.0f);
}

TEST_F(ViewerModelTest, Test_3) {
  s21::ViewerModel model;
  model.setProjection(Parallel);
  EXPECT_EQ(model.getAffineTransform().projectionType, Parallel);
  model.setProjection(Perspective);
  EXPECT_EQ(model.getAffineTransform().projectionType, Perspective);
}

TEST_F(ViewerModelTest, Test_4) {
  s21::ViewerModel model;
  model.setProjection(Parallel);
  model.scaleFigure(scalePlus, 0);
  model.scaleFigure(scalePlus, 2);
  EXPECT_EQ(model.getAffineTransform().scaleFactor, 3.1f);
  model.scaleFigure(scaleMinus, 0);
  model.scaleFigure(scaleMinus, 2);
  EXPECT_EQ(model.getAffineTransform().scaleFactor, 1);

  model.setProjection(Perspective);
  model.scaleFigure(scalePlus, 0);
  model.scaleFigure(scalePlus, 2);
  EXPECT_EQ(model.getAffineTransform().scaleFactor, 1);
  model.scaleFigure(scaleMinus, 0);
  model.scaleFigure(scaleMinus, 2);
  EXPECT_EQ(model.getAffineTransform().scaleFactor, 1);
  model.setDefault(1);
}

TEST_F(ViewerModelTest, Test_SetFacetWidth) {
  s21::ViewerModel model;
  model.setFacetWidth(scalePlus);
  EXPECT_EQ(model.getModelDefinition().facetWidth, 0.005f);
  model.setFacetWidth(scalePlus);
  EXPECT_EQ(model.getModelDefinition().facetWidth, 0.0055f);
  model.setDefault(1);
  model.setFacetWidth(scaleMinus);
  EXPECT_EQ(model.getModelDefinition().facetWidth, 0.005f);
  model.setFacetWidth(scaleMinus);
  EXPECT_EQ(model.getModelDefinition().facetWidth, 0.0045f);
}

TEST_F(ViewerModelTest, Test_Strategy) {
  s21::ViewerModel model;
  QColor color;
  QPoint delta(1, 1);
  model.setFacetColor(color);
  model.setVerticeColor(color);
  model.setBackGroundColor(color);
  model.setVerticeWidth(scalePlus);
  model.setVerticeWidth(scaleMinus);
  model.setVerticeType(Square);
  EXPECT_EQ(model.getModelDefinition().verticeType, Square);
  model.MouseButtonMove(delta);
  EXPECT_EQ(model.getAffineTransform().rotateAngleX, 1);
  EXPECT_EQ(model.getAffineTransform().rotateAngleY, 1);
  model.MouseWheelMove(delta);
  EXPECT_EQ(model.getAffineTransform().rotateAngleZ, 1);
}

TEST_F(ViewerModelTest, loadobj) {
  s21::ViewerModel model;
  model.loadOBJ("../samples/boat.obj");
  QString text = "123.45";
  float float_num = model.makeFloat(text);
  EXPECT_EQ(float_num, 123.45f);
}