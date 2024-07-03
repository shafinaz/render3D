#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QList>
#include <QGraphicsWidget>
#include <QGraphicsLineItem>
#include <QPainter>
#include <QFile>
#include <QTextStream>

#define scaleX 64 //640/10
#define scaleY 48 //480/10

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class vertex;
class edge;
class graph;
}
QT_END_NAMESPACE

/*
 * Class : vertex
 * Description : This class is to set nodes on the scene
 * Members : retPos
 */
class vertex : public QGraphicsEllipseItem
{
public:
    vertex(qreal x = 0, qreal y = 0);
    QPointF retPos() const;

private:

    QPointF newPos;
    QGraphicsWidget *graph;
};
/*
 * Class : edge
 * Description : This class is to set edges that connects vertex class objects
 * Members : paint
 */
class edge : public QGraphicsLineItem
{
public:
    edge(vertex *sourceVx, vertex *destVx);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QPointF sourcePt;
    QPointF destPt;
    qreal arrowsize = 10;

};
/*
 * Class : graph
 * Description : This class is to set the Graphic View and Scene for the Widgets to be projected on.
 * Members : addVertex, addEdge, edges, vertices, edges
 */
class graph : public QGraphicsView
{
    Q_OBJECT

public:
    graph(QGraphicsView *view, QWidget *parent = nullptr);
    void addVertex(int x, int y);
    void addEdge(vertex *v1, vertex *v2);
    QList<vertex*> vertices() const;
    QList<edge*> edges() const;

private:
    QGraphicsScene *scene;
    void drawAxis();
    QList<vertex*> vertexList;
    QList<edge*> edgeList;
};
/*
 * Class : MainWindow
 * Description : This class takes in the generated elements from ui and initializes vertex & edges
 * Members : -
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, const QString &filename = QString());
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    graph *graphView;
    void parseGraphFile(const QString &filename);
};

#endif // MAINWINDOW_H
