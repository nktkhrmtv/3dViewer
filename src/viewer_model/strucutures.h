#include <GL/gl.h>
#include <GL/glut.h>

#include <QApplication>
#include <QButtonGroup>
#include <QColorDialog>
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QtOpenGL>
#include <algorithm>
#include <fstream>
#include <vector>

typedef enum ProjectionType { Parallel, Perspective } ProjectionType_t;

typedef enum VerticeType { Square = 0, Circle, None } VerticeType_t;

typedef struct AffineTransform {
  ProjectionType projectionType = Parallel;
  float rotateAngleX = 0.0f;
  float rotateAngleY = 0.0f;
  float rotateAngleZ = 0.0f;
  float scaleFactor = 1.0f;
  float translateX = 0.0f;
  float translateY = 0.0f;  // To-DO смещение по Y
  float translateZ = 0.0f;  // To-DO смещение по Z
  float fov = 45.0f;
} AffineTransform_t;

typedef struct ModelDefinition {
  QColor facetColor;
  float facetWidth = 0;
  QColor verticeColor;
  VerticeType_t verticeType = Square;
  float verticeWidth = 5.0f;
  QColor backgroundColor;
} ModelDefinition_t;

typedef enum translateAction {
  translateXPlus = 0,
  translateXMinus,
  translateYPlus,
  translateYMinus,
  translateZPlus,
  translateZMinus
} translateAction_t;

typedef enum rotateAction {
  rotateXPlus = 0,
  rotateXMinus,
  rotateYPlus,
  rotateYMinus,
  rotateZPlus,
  rotateZMinus
} rotateAction_t;

typedef enum ScaleType { scalePlus, scaleMinus } ScaleType_t;
