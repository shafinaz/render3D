#include "mainwindow.h"
#include "./ui_mainwindow.h"

/*
 * Class : graph
 * Member : graph
 * Description : This method is a constructor for class graph.
 */
graph::graph(QWidget *parent): QGraphicsView(parent), scene(new QGraphicsScene(this))
{
    setupScene();
    drawAxis();
}

/*
 * Class : graph
 * Member : setupScene
 * Description : This method defines the size of the scene where elements will be projected.
 */
void graph::setupScene()
{
    scene->setSceneRect(-330.5, -235.5, 661, 471);
    setScene(scene);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setMinimumSize(400, 400);
}

/*
 * Class : graph
 * Member : drawAxis
 * Description : This method plots out the axis on the scene.
 */
void graph::drawAxis()
{
    QGraphicsLineItem *xAxis = new QGraphicsLineItem(-330.5, 0, 330.5, 0);  // X axis
    QGraphicsLineItem *yAxis = new QGraphicsLineItem(0, -235.5, 0, 235.5);  // Y axis

    QPen axisPen(Qt::white);  // Pen for axis lines (black color)
    xAxis->setPen(axisPen);
    yAxis->setPen(axisPen);

    scene->addItem(xAxis);
    scene->addItem(yAxis);
}

/*
 * Class : graph
 * Member : vertices
 * Description : This method returns the saved list of vertices.
 */
QList<vertex*> graph::vertices() const
{
    return vertexList;
}

/*
 * Class : graph
 * Member : edges
 * Description : This method returns the saved list of edges.
 */
QList<edge*> graph::edges() const
{
    return edgeList;
}

/*
 * Class : graph
 * Member : addVertex
 * Description : This method adds a vertex in the list.
 */
void graph::addVertex(int x, int y)
{
    vertex *v = new vertex(x, y);
    vertexList.append(v);
    scene->addItem(v);
}

/*
 * Class : graph
 * Member : addEdge
 * Description : This method adds an edge in the list.
 */
void graph::addEdge(vertex *v1, vertex *v2)
{
    edge *e = new edge(v1, v2);
    edgeList.append(e);
    scene->addItem(e);
}

/*
 * Class : vertex
 * Member : vertex
 * Description : This method is a constructor for class vertex.
 */
vertex::vertex(qreal x, qreal y)
    : QGraphicsEllipseItem(x, y, 7, 5)
{
    setBrush(Qt::blue); // Set brush color
    setPen(QPen(Qt::black)); // Set outline pen
    newPos = QPointF(x, y);
    setZValue(-1);
}

/*
 * Class : vertex
 * Member : retPos
 * Description : This method returns the position of a defined vertex.
 */
QPointF vertex::retPos() const
{
    return newPos;
}

/*
 * Class : edge
 * Member : edge
 * Description : This method is a constructor for class edge.
 */
edge::edge(vertex *sourceVx, vertex *destVx)
    : QGraphicsLineItem()
{
    if (!sourceVx || !destVx)
        return;

    setLine(sourceVx->retPos().x(), sourceVx->retPos().y(), destVx->retPos().x(), destVx->retPos().y());

}

/*
 * Class : edge
 * Member : paint
 * Description : This method sets the attributes of the edge line.
 */
void edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(Qt::white);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawLine(line());
}

/*
 * Class : MainWindow
 * Member : MainWindow
 * Description : This method is a constructor for class MainWindow.
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //scene = new graph(this);
    graphView = new graph(this);

    setCentralWidget(graphView);

    //Create vertices
    v1 = new vertex(50, 50);
    v2 = new vertex(150, 150);
    v3 = new vertex(100, 150);

    graphView->addVertex(50, 50);
    graphView->addVertex(150, 150);
    graphView->addVertex(100, 150);

    // Add edges
    graphView->addEdge(v1, v3);
    graphView->addEdge(v2, v3);
    graphView->addEdge(v1, v2);
}

/*
 * Class : MainWindow
 * Member : MainWindow
 * Description : This method is a destructor for class MainWindow.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

