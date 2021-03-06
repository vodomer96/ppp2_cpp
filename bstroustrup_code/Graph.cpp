#include "Graph.h"
#include <map>
#include <cassert>

namespace Graph_lib {

void Shape::draw_lines() const
{
	if (color().visibility() && 1<points.size())	// draw sole pixel?
        for (unsigned int i=1; i<points.size(); ++i)
            fl_line(points[i-1].x,points[i-1].y,points[i].x,points[i].y);
}

void Shape::draw() const
{
	Fl_Color oldc = fl_color();
	// there is no good portable way of retrieving the current style
	fl_color(lcolor.as_int());
	fl_line_style(ls.style(),ls.width());
	draw_lines();
	fl_color(oldc);	// reset color (to pevious) and style (to default)
	fl_line_style(0);
}

//-----------------------------------------------------------------------------

/*bool is_kind_lines(const string& ss)
{
    return (ss == "au" || ss == "ad"
                       || ss == "l");
}*/

//-----------------------------------------------------------------------------

Binary_tree::Binary_tree(Point p, int ll, int ww, const string& ss)
    : l{ll}, dlw{ww}, ke{ss}
{
    if (dlw < 0)
        error("Bad Binary_tree: non-positive down_level_width");
    if (l < 0)
        error("Bad Binary_tree: no such level");
    /*if (!is_kind_lines(kl))
        error("Bad Binary_tree: don't known kind lines!");*/
    add(p);
    init();
}

//-----------------------------------------------------------------------------

void Binary_tree::init()
{
    const double delta = (dlw / pow(2.00, l-1)) /  2.00;
    nw = dlw / pow(2.00, l-1) - delta;
    const double yh =  nw + delta;      // Шаг по ОУ
    double coeff = 0.00;                // Расстояние от левой границы
                                        // до узла
    const double half_nw = nw/2.00;
    lab_nodes.push_back("");
    for (int i=0; i < l; ++i)
        for (int j=0; j < pow(2.00, i); ++j) {
            coeff = dlw * (1+j*2) / pow(2.00, (i+1));
            add(Point{point(0).x - half_nw + coeff,
                      point(0).y + i*yh});
            lab_nodes.push_back("");
        }
    choose_kind_edges();
}

//-----------------------------------------------------------------------------

void Binary_tree::add_text_node(const string& loc,
                                const string& lab)
{    
    if (loc.size() > l)
        error("Binary_tree::add_text_node:",
              "don't correct location nodes");
    if (loc.front() != 'b')
        error("Binary_tree::add_text_node:",
              "location must have symbol base");

    static constexpr int lcoeff = 0;
    static constexpr int rcoeff = 1;

    int counter = 1;
    istringstream is{loc};
    for (char ch = 0; is >> ch;) {
        switch (ch) {
        case 'b':
            break;
        case 'l':
            counter *= 2;
            counter += lcoeff;
            break;
        case 'r':
            counter *= 2;
            counter+=rcoeff;
            break;
        default:
            error("Binary_tree::add_text_node:",
                  "don't known symbol location");
            break;
        }

    }
    lab_nodes[counter] = lab;
}

//-----------------------------------------------------------------------------

void draw_label(Point xy, const string& lab)
{
    static const int dx = 4;
    static const int dy = 4;

    fl_draw(lab.c_str(),xy.x-dx,xy.y+dy);
}

//-----------------------------------------------------------------------------

Arrow* get_arrow(Point b, Point e, char c)
{
    return (c == 'd') ? new Arrow{b,e}
                      : new Arrow{e,b};
}

//-----------------------------------------------------------------------------

void Binary_tree::choose_kind_edges()
{
    const double r = nw/2;
    int half_np = number_of_points()/2;
    int counter = 2;
    Point b{0, 0};
    Point e{0, 0};
    switch (ke.front()) {
    case 'a':
        for (int i=1; i < half_np; ++i) {
            for (int j=0; j < 2; ++j) {
                if (j==0) { // Соединение с левым узлом
                    b = Point{point(i).x+r/2.00,point(i).y+r+r};
                    e = Point{point(counter).x+r+r/2.00,point(counter).y};
                }
                else {      // Соединение с правым узлом
                    b = Point{point(i).x+r+r/2.00,point(i).y+r+r};
                    e = Point{point(counter).x+r-r/2.00,point(counter).y};
                }
                edges.add(get_arrow(b, e, ke[1]));
                ++counter;
            }
        }
        break;
    case 'l':
        for (int i=1; i < half_np; ++i) {
            for (int j=0; j < 2; ++j) {
                if (j==0) {
                    b = Point{point(i).x+r/2.00,point(i).y+r+r};
                    e = Point{point(counter).x+r+r/2.00,point(counter).y};
                }
                else {
                    b = Point{point(i).x+r+r/2.00,point(i).y+r+r};
                    e = Point{point(counter).x+r-r/2.00,point(counter).y};
                }
                edges.add(new Line(b, e));
                ++counter;
            }
        }
        break;
    }
}

//-----------------------------------------------------------------------------

void Binary_tree::set_color_edges(Color col)
{
    for (int i=0; i < edges.size(); ++i)
        edges[i].set_color(col);
}

//-----------------------------------------------------------------------------

void Binary_tree::move(int dx, int dy)
{
    Shape::move(dx, dy);
    for (int i=0; i < edges.size(); ++i)
        edges[i].move(dx, dy);
}

//-----------------------------------------------------------------------------

void Binary_tree::draw_lines() const
{
    for (int i=0; i < edges.size(); ++i)
        edges[i].draw();

    draw_nodes();

    for (int i = 1; i < number_of_points(); ++i)
        if (lab_nodes[i] != "")
            draw_label(point(i), lab_nodes[i]);
}

//-----------------------------------------------------------------------------

void Binary_tree::draw_nodes() const
{
    const double r = nw/2.00;
    if (fill_color().visibility()) {
        fl_color(fill_color().as_int());
        for (int i=1; i < number_of_points(); ++i)
            fl_pie(point(i).x, point(i).y, r+r-1, r+r-1, 0, 360);
        fl_color(color().as_int());
    }

    if (color().visibility())
        for (int i=1; i < number_of_points(); ++i)
            fl_arc(point(i).x, point(i).y, r+r, r+r, 0, 360);

}

//-----------------------------------------------------------------------------

void Triangle_nodes_binary_tree::draw_nodes() const
{
    const double r = node_width()/2.00;
    if (fill_color().visibility()) {
        fl_color(fill_color().as_int());
        for(int i=1; i<number_of_points(); ++i){
            fl_begin_complex_polygon();
            fl_vertex(point(i).x+r, point(i).y);
            fl_vertex(point(i).x,point(i).y+r+r);
            fl_vertex(point(i).x+r+r,point(i).y+r+r);
            fl_end_complex_polygon();
        }
        fl_color(color().as_int());
    }

    if (color().visibility())
        for (int i=1; i < number_of_points(); ++i) {
            fl_line(point(i).x+r, point(i).y,
                    point(i).x,point(i).y+r+r);
            fl_line(point(i).x,point(i).y+r+r,
                    point(i).x+r+r,point(i).y+r+r);
            fl_line(point(i).x+r+r,point(i).y+r+r,
                    point(i).x+r, point(i).y);
        }
}

//-----------------------------------------------------------------------------

/*void Triangle_nodes_binary_tree::draw_lines() const
{
    const double r = node_width()/2.00;
    int counter = 2;
    int half_np = number_of_points()/2;
    for (int i=1; i < half_np; ++i) {
        for (int j=0; j < 2; ++j) {
            if (j==0)
                fl_line(point(i).x+r/2.0,point(i).y+r+r,
                        point(counter).x+r,point(counter).y);
            else
                fl_line(point(i).x+r+r/2.0,point(i).y+r+r,
                        point(counter).x+r,point(counter).y);
            ++counter;
        }
    }
    draw_nodes();
}*/

//-----------------------------------------------------------------------------
// does two lines (p1,p2) and (p3,p4) intersect?
// if se return the distance of the intersect point as distances from p1
inline pair<double,double> line_intersect(Point p1, Point p2, Point p3, Point p4,
                                          bool& parallel)
{
    double x1 = p1.x;
    double x2 = p2.x;
	double x3 = p3.x;
	double x4 = p4.x;
	double y1 = p1.y;
	double y2 = p2.y;
	double y3 = p3.y;
	double y4 = p4.y;

	double denom = ((y4 - y3)*(x2-x1) - (x4-x3)*(y2-y1));
	if (denom == 0){
		parallel= true;
		return pair<double,double>(0,0);
	}
	parallel = false;
	return pair<double,double>( ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3))/denom,
								((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3))/denom);
}

//intersection between two line segments
//Returns true if the two segments intersect,
//in which case intersection is set to the point of intersection
bool line_segment_intersect(Point p1, Point p2, Point p3, Point p4,
                            Point& intersection){
   bool parallel;
   pair<double,double> u = line_intersect(p1,p2,p3,p4,parallel);
   if (parallel || u.first < 0 || u.first > 1 || u.second < 0 || u.second > 1) return false;
   intersection.x = p1.x + u.first*(p2.x - p1.x);
   intersection.y = p1.y + u.first*(p2.y - p1.y);
   return true;
}

void Polygon::add(Point p)
{
	int np = number_of_points();

	if (1<np) {	// check that thenew line isn't parallel to the previous one
		if (p==point(np-1)) error("polygon point equal to previous point");
		bool parallel;
		line_intersect(point(np-1),p,point(np-2),point(np-1),parallel);
		if (parallel)
			error("two polygon points lie in a straight line");
	}

	for (int i = 1; i<np-1; ++i) {	// check that new segment doesn't interset and old point
		Point ignore(0,0);
		if (line_segment_intersect(point(np-1),p,point(i-1),point(i),ignore))
			error("intersect in polygon");
	}


	Closed_polyline::add(p);
}


void Polygon::draw_lines() const
{
		if (number_of_points() < 3) error("less than 3 points in a Polygon");
		Closed_polyline::draw_lines();
}

//-----------------------------------------------------------------------------
// Exercise 13_18
Poly::Poly(initializer_list<Point> lst)
{
    for (const Point& p : lst ) {
        int np = number_of_points();

        if (1<np) {	// check that thenew line isn't parallel to the previous one
            if (p==point(np-1)) error("polygon point equal to previous point");
            bool parallel;
            line_intersect(point(np-1),p,point(np-2),point(np-1),parallel);
            if (parallel)
                error("two polygon points lie in a straight line");
        }

        for (int i = 1; i<np-1; ++i) {	// check that new segment doesn't interset and old point
            Point ignore(0,0);
            if (line_segment_intersect(point(np-1),p,point(i-1),point(i),ignore))
                error("intersect in polygon");
        }
        Closed_polyline::add(p);
    }
}

//-----------------------------------------------------------------------------
// Exercise 13_18
void Poly::draw_lines() const
{
    if (number_of_points() < 3) error("less than 3 points in a Polygon");
    Closed_polyline::draw_lines();
}

//-----------------------------------------------------------------------------

void Open_polyline::draw_lines() const
{
		if (fill_color().visibility()) {
			fl_color(fill_color().as_int());
			fl_begin_complex_polygon();

            for(int i=0; i<number_of_points(); ++i)
				fl_vertex(point(i).x, point(i).y);

			fl_end_complex_polygon();
			fl_color(color().as_int());	// reset color
		}

		if (color().visibility())
			Shape::draw_lines();
}


void Closed_polyline::draw_lines() const
{
	Open_polyline::draw_lines();

	if (color().visibility())	// draw closing line:
		fl_line(point(number_of_points()-1).x,point(number_of_points()-1).y,point(0).x,point(0).y);
}

//-----------------------------------------------------------------------------
// Получаем координаты верхней левой и
// правой нижней точки наименьшего прямоугольника,
// окамляющего наш закрытый контур
pair<Point, Point> Striped_closed_polyline::get_out_rectangle() const
{
    const int& np = number_of_points();
    vector<int> ys;
    vector<int> xs;
    for (int i=0; i < np; ++i) {
        xs.push_back(point(i).x);
        ys.push_back(point(i).y);
    }

    sort(ys.begin(), ys.end());
    sort(xs.begin(), xs.end());

    return pair<Point, Point>(Point{xs.front(), ys.front()},
                              Point{xs.back(),  ys.back()});
}

//-----------------------------------------------------------------------------
// Поиск x-точек пересечения между
// line и всеми отрезками полигона
void Striped_closed_polyline::getx_instersect(const pair<Point, Point>& line,
                                              vector<int>& xs) const
{
    const int& np = number_of_points();
    Point intrsct{0, 0};
    for (int j = 1; j < np; ++j) {
        intrsct = Point{0, 0};
        if (line_segment_intersect(line.first, line.second,
                                   point(j-1), point(j), intrsct))
            xs.push_back(intrsct.x);
    }
    // Пересекает ли прямая замыкающий отрезок?
    if (line_segment_intersect(line.first, line.second,
                               point(0),   point(np-1), intrsct))
        xs.push_back(intrsct.x);
    sort(xs.begin(), xs.end());
}

//-----------------------------------------------------------------------------

void Striped_closed_polyline::move(int dx, int dy)
{
    Shape::move(dx, dy);
    orect.first.x += dx;
    orect.first.y += dy;
    orect.second.x += dx;
    orect.second.y += dy;
}

//-----------------------------------------------------------------------------

void Striped_closed_polyline::draw_lines() const
{        
    const int dy = style().width() + 4;

    int xmin = orect.first.x;
    int ymin = orect.first.y + dy;
    int xmax = orect.second.x;
    int ymax = orect.second.y;

    vector<int> xs;    
    pair<Point, Point> line = pair<Point, Point>(Point{0, 0}, Point{0, 0});
    if (fill_color().visibility()) {    // Заполнение внутренней области
        fl_color(fill_color().as_int());
        for (int i = ymin; i < ymax; i+=dy) {   // Ищем координаты штрихов
            // Задаем отрезок, параллельный ОХ
            line = pair<Point, Point>(Point{xmin, i}, Point{xmax, i});
            getx_instersect(line, xs);  // Получаем x-точек пересечения между
                                        // line и всех отрезков полигона
            for (int k=1; k < xs.size(); k+=2)
                fl_line(xs[k-1], i, xs[k], i);  // Соединяем отрезки штриховки
            xs.clear();
        }
        fl_color(color().as_int());
    }
    Shape::draw_lines();
    if (color().visibility())	// draw closing line:
        fl_line(point(number_of_points()-1).x, point(number_of_points()-1).y,
                point(0).x,                    point(0).y);
}

//-----------------------------------------------------------------------------

void Shape::move(int dx, int dy)
{
	for (unsigned int i = 0; i<points.size(); ++i) {
		points[i].x+=dx;
		points[i].y+=dy;
	}
}

void Lines::draw_lines() const
{
//	if (number_of_points()%2==1) error("odd number of points in set of lines");
	if (color().visibility())
		for (int i=1; i<number_of_points(); i+=2)
			fl_line(point(i-1).x,point(i-1).y,point(i).x,point(i).y);
}

void Text::draw_lines() const
{
	int ofnt = fl_font();
	int osz = fl_size();
	fl_font(fnt.as_int(),fnt_sz);
	fl_draw(lab.c_str(), point(0).x, point(0).y);
	fl_font(ofnt,osz);
}


// the function parameters are not stored
Function::Function(Fct f, double r1, double r2, Point xy, int count,
         double xscale, double yscale)
{
    if (r2-r1<=0) error("bad graphing range");
    if (count<=0) error("non-positive graphing count");
    double dist = (r2-r1)/count;
    double r = r1;
    for (int i = 0; i<count; ++i) {
        add(Point{xy.x+int(r*xscale),xy.y-int(f(r)*yscale)});
        r += dist;
    }
}

//-----------------------------------------------------------------------------

Function::Function(Fct_capture f, double r1, double r2, Point xy, int count,
         double xscale, double yscale)
{
    if (r2-r1<=0) error("bad graphing range");
    if (count<=0) error("non-positive graphing count");
    double dist = (r2-r1)/count;
    double r = r1;
    for (int i = 0; i<count; ++i) {
        add(Point{xy.x+int(r*xscale),xy.y-int(f(r)*yscale)});
        r += dist;
    }
}

//-----------------------------------------------------------------------------

bool is_funct(double rmin, double rmax, int count)
{
    if (rmax-rmin<=0) return false;
    if (count<=0)     return false;
    return true;
}

//-----------------------------------------------------------------------------

const string& error_funct()
{
    static string err = "Don't correct Funct";
    return err;
}

//-----------------------------------------------------------------------------

Funct::Funct(Fct ff, double r_min, double r_max, Point orig, int count,
             double xscale, double yscale)
    : f{ff}, r1{r_min}, r2{r_max}, xy{orig}, cp{count},
      xs{xscale}, ys{yscale}, fc {nullptr}
{
    if (!is_funct(r_min, r_max, count))
        error(error_funct());
    recalculate();
}

//-----------------------------------------------------------------------------

Funct::Funct(Fct_capture ff, double r_min, double r_max, Point orig,
             int count, double xscale, double yscale)
    : fc{ff}, r1{r_min}, r2{r_max}, xy{orig}, cp{count},
      xs{xscale}, ys{yscale}, f{nullptr}
{
    if (!is_funct(r_min, r_max, count))
        error("Funct::Funct:", error_funct());
    recalculate();
}

//-----------------------------------------------------------------------------

void Funct::set_r_min(double r_min)
{
    if (!is_funct(r_min, r2, cp))
        error("Funct::set_r_min:", error_funct());
    r1 = r_min;
    recalculate();
}

//-----------------------------------------------------------------------------

void Funct::set_r_max(double r_max)
{
    if (!is_funct(r1, r_max, cp))
        error("Funct::set_r_max:", error_funct());
    r2 = r_max;
    recalculate();
}

//-----------------------------------------------------------------------------

void Funct::set_count_of_points(int count)
{
    if (!is_funct(r1, r2, count))
        error("Funct::set_count_of_points:", error_funct());
    cp = count;
    recalculate();
}

//-----------------------------------------------------------------------------

void Funct::recalculate()
{
    clear_points();
    double dist = (r2-r1)/cp;
    double r = r1;
    for (int i = 0; i < cp; ++i) {
        if (fc == nullptr)
            add(Point{xy.x + int(r*xs),
                      xy.y - int(f(r)*ys)});
        else
            add(Point{xy.x + int(r*xs),
                      xy.y - int(fc(r)*ys)});
        r += dist;
    }
}

//-----------------------------------------------------------------------------

void Funct_precision::recalculate()
{
    clear_points();
    double dist = (r_max()-r_min())/count_of_points();
    double r = r_min();
    Fct* f = function();
    Fct_capture fc = function_capture();
    double pr = precision();
    for (int i = 0; i < count_of_points(); ++i) {
        if (fc == nullptr)
            add(Point{orig().x + int(int(r*xscale())/pr)*pr,
                      orig().y - int(int(f(r)*yscale())/pr)*pr});
        else
            add(Point{orig().x + int(int(r*xscale())/pr)*pr,
                      orig().y - int(int(fc(r)*yscale())/pr)*pr});
        r += dist;
    }
}

//-----------------------------------------------------------------------------

bool is_column_chart(double w, double h)
{
    return (h<=0 || w<=0) ? false : true;

}

//-----------------------------------------------------------------------------

Column_chart::Column_chart(Point xy, double ww, double hh,
                           double wscale, double hscale)
    : w{ww*wscale}, ws{wscale}, hs {hscale}
{
    hh *= hs;
    if(!is_column_chart(w, hh))
        error("add(): Invalid column_chart");

    Shape::add(xy); // Начало координат
    Shape::add(Point{point(0).x, point(0).y - hh}); // Верхняя левая точка
                                                    // первого столбца
    vh.push_back(hh);
}

//-----------------------------------------------------------------------------

void Column_chart::add(double height)
{
    height *= hs;
    if(!is_column_chart(w, height))
        error("add(): Invalid column_chart");

    Shape::add(Point{point(number_of_points()-1).x + w,
                     point(0).y - height});
    vh.push_back(height);

}

//-----------------------------------------------------------------------------

void Column_chart::draw_lines() const
{
    int np = number_of_points();
    if (fill_color().visibility()) {	// fill
        fl_color(fill_color().as_int());
        for (int i=1; i < np; ++i)
            fl_rectf(point(i).x, point(i).y,w,vh[i-1]);
        fl_color(color().as_int());	// reset color
    }

    if (color().visibility()) {	// edge on top of fill
        fl_color(color().as_int());
        for (int i=1; i < np; ++i)
            fl_rect(point(i).x, point(i).y,w,vh[i-1]);
    }
}

//-----------------------------------------------------------------------------

void Marked_column_chart::draw_lines() const
{
    Column_chart::draw_lines();
    for (int i = 0; i < marks.size(); ++i)
        marks[i].draw();
}

//-----------------------------------------------------------------------------

void Marked_column_chart::move(int dx, int dy)
{
    Column_chart::move(dx, dy);
    for (int i = 0; i < marks.size(); ++i) marks[i].move(dx, dy);
}

//-----------------------------------------------------------------------------

void Marked_column_chart::set_color(Color col)
{
    Column_chart::set_color(col);
    for (int i = 0; i < marks.size(); ++i) marks[i].set_color(col);
}

//-----------------------------------------------------------------------------

void Rectangle::draw_lines() const
{
    if (fill_color().visibility()) {	// fill
		fl_color(fill_color().as_int());
		fl_rectf(point(0).x,point(0).y,w,h);
		fl_color(color().as_int());	// reset color
    }

	if (color().visibility()) {	// edge on top of fill
		fl_color(color().as_int());
        fl_rect(point(0).x,point(0).y,w,h);
	}
}

//-----------------------------------------------------------------------------

void get_item_board(Point p, vector<Point>& vi, int h)
{
    constexpr int item_on_line = 8;
    const int item_size = h / item_on_line;
    const int item_line_size = item_on_line * item_size;
    const int delta = (h - item_line_size)/ 2;

    for (int y=0; y < item_line_size; y += item_size)
        for (int x=0; x < item_line_size; x += item_size)
            vi.push_back(Point{p.x + x + delta,
                               p.y + y + delta});
}

//-----------------------------------------------------------------------------

Chess_board::Chess_board(Point xy, int rr)
    : Rectangle(xy, rr, rr)
{    
    if (rr < min_size) error("not correct size");
    init();
}

//-----------------------------------------------------------------------------

void Chess_board::init()
{
    const int item_size = height() / item_on_line;     // Ширина ячейки
    const int item_line_size = item_on_line*item_size; // Ширина строки ячеек
    const int delta = (height() - item_line_size) / 2; // Погрешность
    const int checkers_rds = item_size/2-item_size/10; // Радиус пешки
    const int icntr = item_size/2;                     // Центр ячейки и пешки
    constexpr pair<int,int> empty(3, 4);               // Строки без пешек

    set_fill_color(Color::visible);

    int parity = 0;
    int row = 0;
    for (int y=0; y < item_line_size; y += item_size) {
        for (int x=0; x < item_line_size; x += item_size) {
            // Добавляем ячейки
            add(Point{point(0).x + x + delta, point(0).y + y + delta});
            if (parity%2 != 0 &&
               (row < empty.first || row > empty.second))   // Добавляем пешки
                checkers.add(new Circle{
                                 Point{point(number_of_points()-1).x + icntr,
                                       point(number_of_points()-1).y + icntr},
                                       checkers_rds});
            ++parity;
        }
        ++parity;
        ++row;
    }
    set_color_up_checkers(col_up);
    set_color_down_checkers(col_down);
}

//-----------------------------------------------------------------------------

void Chess_board::move(int dx, int dy)
{
    Shape::move(dx, dy);
    for (int i=0; i < checkers.size(); ++i)
        checkers[i].move(dx, dy);
}

//-----------------------------------------------------------------------------

void Chess_board::set_color_up_checkers(Color col)
{
    col_up = col;
    for (int i=0; i < checkers.size()/2; ++i) {
        checkers[i].set_color(col_up);
        checkers[i].set_fill_color(col_up);
    }
}

//-----------------------------------------------------------------------------

void Chess_board::set_color_down_checkers(Color col)
{
    col_down = col;
    for (int i=checkers.size()/2; i < checkers.size(); ++i) {
        checkers[i].set_color(col_down);
        checkers[i].set_fill_color(col_down);
    }
}

//-----------------------------------------------------------------------------

void Chess_board::draw_lines() const
{    
    Rectangle::draw_lines();    // Заполняем фон  
    const int item_size = height() / item_on_line;

    if (fill_color().visibility()) {	// fill
        int parity = 0;
        for(int i=1; i < number_of_points(); ++i) {
            if (parity % (item_on_line + 1) == 0)
                ++parity;

            (parity % 2 == 0) ? fl_color(Color::black)  // Какого цвета ячейка?
                              : fl_color(Color::white);

            fl_rectf(point(i).x, point(i).y, item_size, item_size);
            ++parity;
        }
        fl_color(color().as_int());     // reset color
    }

    if (color().visibility()) {
        fl_color(color().as_int());
        for (int i=1; i < number_of_points(); ++i)
            fl_rect(point(i).x, point(i).y, item_size, item_size);
    }
    // Рисуем пешки
    for (int i=0; i < checkers.size(); ++i)
        checkers[i].draw();
}

//-----------------------------------------------------------------------------

void Striped_rectangle::draw_lines() const
{
    if (fill_color().visibility()) {	// fill
        fl_color(fill_color().as_int());

        for (int i=4; i < height(); i+=4)
            fl_line(point(0).x,point(0).y+i,point(0).x+width()-1,point(0).y+i);

        fl_color(color().as_int());	// reset color
    }
    if (color().visibility()) {	// edge on top of fill
        fl_color(color().as_int());
        fl_rect(point(0).x,point(0).y,width(),height());
    }
}

//-----------------------------------------------------------------------------
// Exercise 13_2
// Фигура состоит из 4 отрезков и 4 эллипсов
void Box::init()
{
    const int& rww = width();
    const int& rhh = height();
    const Point& xy = point(0); // Верхняя левая точка
                                // первого эллипса
    // Вершины, определеяющие отрезки
    Point top2{xy.x, xy.y+rhh-eh};
    Point top4{xy.x+rww-ew, xy.y+rhh};
    Point top7{xy.x+rww-ew, xy.y};
    // Остальные верхние левые точки эллипсов
    add(Point{top2.x, top2.y-eh});
    add(Point{top4.x-ew, top4.y-eh-eh});
    add(Point{top7.x-ew, top7.y});
    // Добавляем вершины, определяющие отрезки
    add(Point{xy.x, xy.y+eh});
    add(top2);
    add(Point{xy.x+ew, xy.y+rhh});
    add(top4);
    add(Point{xy.x+rww, xy.y+rhh-eh});
    add(Point{xy.x+rww, xy.y+eh});
    add(top7);
    add(Point{xy.x+ew, xy.y});
}

//-----------------------------------------------------------------------------
// Exercise 13_2
void Box::draw_lines() const
{
    constexpr int left_up_count = 4; // Количество верхних
                                     // левых точек еллипсов
    constexpr int angle = 90;        // Угол увеличения
    if (color().visibility()) {	// edge on top of fill
        fl_color(color().as_int());
        for (int i=0; i < left_up_count; ++i) { // paint ellipses
            if (i==left_up_count-1)
                fl_arc(point(i).x,point(i).y,ew+ew,eh+eh,
                       0, angle);
            else
                fl_arc(point(i).x,point(i).y,ew+ew,eh+eh,
                       (i+1)*angle, (i+2)*angle);
        }
        // paint four lines
        for (int i=left_up_count+1; i<number_of_points(); i+=2)
            fl_line(point(i-1).x,point(i-1).y,point(i).x,point(i).y);
    }
}

//-----------------------------------------------------------------------------
// Exercise 13_3
Arrow::Arrow(Point b, Point e)
{
    set_fill_color(color());
    add(b, e);
    if (l<=0 || t<=0) error("Bad arrow: non-positive side");
    init();
}

//-----------------------------------------------------------------------------
// Exercise 13_3
Arrow::Arrow(Point b, Point e, int ll, int tt)
    : l{ll}, t{tt}
{
    set_fill_color(color());
    add(b, e);
    if (l<=0 || t<=0) error("Bad arrow: non-positive side");
    init();
}

//-----------------------------------------------------------------------------

void Arrow::init()
{
    const Point& b = point(0);
    const Point& e = point(1);
    // Нахождение координат вектора be
    double vect_x = e.x - b.x;
    double vect_y = e.y - b.y;
    double lenght = sqrt(pow(vect_x, 2) + pow(vect_y, 2));  // Длина вектора
    double unit_x = vect_x / lenght;    // x, единичного вектора
    double unit_y = vect_y / lenght;    // y, единичного вектора
    // Находим точку O, через которую пойдет
    // одна из сторон "усика"
    double opoint_x = e.x - l * unit_x;
    double opoint_y = e.y - l * unit_y;
    // Находим вектор нормали,
    // умноженный на поперечную длину "усика"
    double normal_x = t * unit_y;
    double normal_y = t *(-unit_x);
    // Искомые точки "усиков"
    Point a{int(opoint_x + normal_x), int(opoint_y + normal_y)};
    Point c{int(opoint_x - normal_x), int(opoint_y - normal_y)};
    add(a, c); // Формируем отрезки стрелы
    add(c, e);
    add(e, a);
}

void Arrow::draw_lines() const
{
    // Заполняем цветом усик
    if (color().visibility()) {
        fl_color(color().as_int());
        fl_begin_complex_polygon();
        for(int i=2; i<number_of_points(); ++i){
            fl_vertex(point(i).x, point(i).y);
        }
        fl_end_complex_polygon();
    }
    Lines::draw_lines();
}

//-----------------------------------------------------------------------------
// Exercise 13_8
Regular_hexagon::Regular_hexagon(Point xy, int rr)
    : r{rr}
{
    if (r<=0) error("Regular_hexagon: non-positive radius");
    add(xy);
    vector<Point> points;
    find_dot_reg_poly(points, xy, 6, r, 0.00);
    for (int i=0; i < points.size(); ++i)
        add(points[i]);
}

//-----------------------------------------------------------------------------
// Exercise 13_8
void Regular_hexagon::draw_lines() const
{
    if (fill_color().visibility()) {
        fl_color(fill_color().as_int());
        fl_begin_complex_polygon();
        for(int i=1; i<number_of_points(); ++i){
            fl_vertex(point(i).x, point(i).y);
        }
        fl_end_complex_polygon();
        fl_color(color().as_int());	// reset color
    }
    if (color().visibility()) {
        for (int i=2; i<number_of_points(); ++i)
            fl_line(point(i-1).x,point(i-1).y,point(i).x,point(i).y);

        fl_line(point(number_of_points()-1).x,
                point(number_of_points()-1).y,point(1).x,point(1).y);
    }
}

//-----------------------------------------------------------------------------
// Exercise 13_8
void find_dot_reg_poly(vector<Point>& points, Point center,
                       int count_angle, int radius, double rot)
{
    const double delta_angle = 360.00 / count_angle;
    double angle = rot; // Угол пересечения вершины
                        // и описываемой окружности
    for (int i=0; i < count_angle; ++i) {
        points.push_back(find_dot(center, angle, radius));
        angle += delta_angle; // Увеличиваем угол на величину
                              // угла правильного многоугольника
    }
}

//-----------------------------------------------------------------------------
// Exercise 13_8
Point find_dot(Point center, int angle, int radius)
{
    return Point{
        center.x + radius * cos(angle * M_PI/180.0),
        center.y + radius * sin(angle * M_PI/180.0)
    };
}

//-----------------------------------------------------------------------------
// Exercise 13_10
Regular_polygon::Regular_polygon(Point xy, int ss, int rr)
    : r{rr}, s{ss}
{
    if (r<=0) error("Regular_polygon: non-positive radius");
    if (s<=2) error("Regular_polygon: non-correct polygon");
    add(xy);
    vector<Point> points;
    find_dot_reg_poly(points, xy, s, r, 0.00);
    for (int i=0; i < points.size(); ++i)
        add(points[i]);
}

//-----------------------------------------------------------------------------
// Exercise 13_10
void Regular_polygon::draw_lines() const
{
    if (fill_color().visibility()) {
        fl_color(fill_color().as_int());
        fl_begin_complex_polygon();
        for(int i=1; i<number_of_points(); ++i){
            fl_vertex(point(i).x, point(i).y);
        }
        fl_end_complex_polygon();
        fl_color(color().as_int());	// reset color
    }
    if (color().visibility()) {
        for (int i=2; i<number_of_points(); ++i)
            fl_line(point(i-1).x,point(i-1).y,point(i).x,point(i).y);

        fl_line(point(number_of_points()-1).x,
                point(number_of_points()-1).y,point(1).x,point(1).y);
    }
}

//-----------------------------------------------------------------------------

void find_dot_star(vector<Point>& points, Point cntr, int R, int r, int n)
{
    int a=0;
    for (int i=1; i < n*2+2; ++i) {
        if (!(i%2)) // Четно?
            points.push_back(Point{cntr.x+r/2*cos(a*M_PI/180),
                                   cntr.y-r/2*sin(a*M_PI/180)});
        else
            points.push_back(Point{cntr.x+R*cos(a*M_PI/180),
                                   cntr.y-R*sin(a*M_PI/180)});
        a=a+180/n;
    }
}

//-----------------------------------------------------------------------------
// Exercise 13_19
Star::Star(Point xy, int nn, int irds, int ords)
    :n{nn}, in_rds{irds}, out_rds{ords}
{
    if (in_rds<=0 || out_rds<=0)
        error("Regular_polygon: non-positive radius");
    if (n<=2) error("Regular_polygon: non-correct polygon");
    add(xy);
    vector<Point> points;
    find_dot_star(points, xy, out_rds, in_rds, n);
    for (int i=0; i < points.size(); ++i)
        add(points[i]);
}

//-----------------------------------------------------------------------------
// Exercise 13_10
void Star::draw_lines() const
{
    if (fill_color().visibility()) {
        fl_color(fill_color().as_int());
        fl_begin_complex_polygon();
        for(int i=1; i<number_of_points(); ++i){
            fl_vertex(point(i).x, point(i).y);
        }
        fl_end_complex_polygon();
        fl_color(color().as_int());	// reset color
    }
    if (color().visibility()) {
        for (int i=2; i<number_of_points(); ++i)
            fl_line(point(i-1).x,point(i-1).y,point(i).x,point(i).y);

        fl_line(point(number_of_points()-1).x,
                point(number_of_points()-1).y,point(1).x,point(1).y);
    }
}

//-----------------------------------------------------------------------------
// Exercise 13_14
Right_triangle::Right_triangle(Point xy, int cc1, int cc2)
    : c1{cc1}, c2{cc2}
{
    if (c1==0 || c2==0)
        error("Катет не может иметь нулевую длину");
    add(xy);
    add(Point{xy.x + c1, xy.y});
    add({xy.x, xy.y + c2});
}

//-----------------------------------------------------------------------------

void Right_triangle::draw_lines() const
{
    if (fill_color().visibility()) {
        fl_color(fill_color().as_int());
        fl_begin_complex_polygon();
        for(int i=0; i<number_of_points(); ++i){
            fl_vertex(point(i).x, point(i).y);
        }
        fl_end_complex_polygon();
        fl_color(color().as_int());	// reset color
    }

    if (color().visibility()) {
        for (int i=1; i<number_of_points(); ++i)
            fl_line(point(i-1).x,point(i-1).y,point(i).x,point(i).y);

        fl_line(point(number_of_points()-1).x,
                point(number_of_points()-1).y,point(0).x,point(0).y);
    }
}

//-----------------------------------------------------------------------------

Axis::Axis(Orientation d, Point xy, int length,
           int n, string lab) : label{Point{0, 0}, lab}
{
    if (length < 0) error("Неверная длина оси");
    switch (d) {
    case Axis::x:
    {
        Shape::add(xy); // Линия Оси
        Shape::add(Point{xy.x+length, xy.y});

        if (n > 0) {    // Добавление делений
            int dist = length / n;
            int x = xy.x + dist;
            for (int i=0; i < n; ++i) {
                notches.add(Point{x, xy.y}, Point{x, xy.y - 5});
                x += dist;
            }
        }
        label.move(length/3, xy.y+20);  // Метка под линией
        break;
    }
    case Axis::y:
    {
        Shape::add(xy); // Линия Оси y
        Shape::add(Point{xy.x, xy.y - length});

        if (0 < n) {    // Добавление делений
            int dist = length / n;
            int y = xy.y - dist;
            for (int i=0; i < n; ++i) {
                notches.add(Point{xy.x, y}, Point{xy.x + 5, y});
                y -= dist;
            }
        }
        label.move(xy.x - 10, xy.y- length - 10);  // Метка вверху
        break;
    }
    case Axis::z:
        error("Ось z не реализована");
    }

}

void Axis::draw_lines() const
{
	Shape::draw_lines();	// the line
	notches.draw();	// the notches may have a different color from the line
	label.draw();	// the label may have a different color from the line
}


void Axis::set_color(Color c)
{
    Shape::set_color(c);
    notches.set_color(c);
    label.set_color(c);
}

void Axis::move(int dx, int dy)
{
	Shape::move(dx,dy);
	notches.move(dx,dy);
	label.move(dx,dy);
}

void Circle::draw_lines() const
{
	if (fill_color().visibility()) {	// fill
		fl_color(fill_color().as_int());
		fl_pie(point(0).x,point(0).y,r+r-1,r+r-1,0,360);
		fl_color(color().as_int());	// reset color
	}

	if (color().visibility()) {
		fl_color(color().as_int());
		fl_arc(point(0).x,point(0).y,r+r,r+r,0,360);
	}
}

//-----------------------------------------------------------------------------
// Возвращает абсциссы точек пересечения штриха-прямой и круга
pair<int,int> getx_ln_intrsct_crcl(Point cntr, int rds, int y)
{
    constexpr double is_pow = 2.0;
    double c = pow(double(cntr.x),is_pow) + pow(double(y-cntr.y), is_pow)
             - pow(double(rds),   is_pow);
    double b = -2.0*cntr.x;
    double d = pow(b, is_pow) - 4*c;

    if(d > 0)
        return pair<int,int>((-b - sqrt(d)) / 2.0,
                             (-b + sqrt(d)) / 2.0);
    error("Whakhanalia");
}

//-----------------------------------------------------------------------------

void Striped_circle::draw_lines() const
{
    const int& r = radius();
    constexpr int delta = 10;
    if (fill_color().visibility()) {	// fill
        fl_color(fill_color().as_int());

        for (int i=delta; i < r+r; i+=delta) {
            auto xs = getx_ln_intrsct_crcl(center(), r, point(0).y+i);
            fl_line(xs.first,point(0).y+i,xs.second,point(0).y+i);
        }

        fl_color(color().as_int());	// reset color
    }
    if (color().visibility()) {
        fl_color(color().as_int());
        fl_arc(point(0).x,point(0).y,r+r,r+r,0,360);
    }

}

//-----------------------------------------------------------------------------
// Возвращает пару точек "левого и правого глаза"
inline pair<Point, Point> get_eye(Point p, int cr, int er)
{
    return pair<Point, Point>({p.x + cr/2.0 - er, p.y + cr/2.0},
                              {p.x + 1.5*cr - er, p.y + cr/2.0});
}

//-----------------------------------------------------------------------------
// Возвращает верхную левую точку "рта"
inline Point get_mouth(Point p, int cr, int sr)
{
    return {p.x + cr - sr,
            p.y + 1.5*cr - sr};
}

//-----------------------------------------------------------------------------

Smiley::Smiley(Point p, int rr)
    : Circle{p, rr}, er{1.0*rr/delim1}, mr{1.0*rr/delim2}
{
    auto eye = get_eye(point(0), radius(), er);
    add(eye.first);
    add(eye.second);
    add(get_mouth(point(0), radius(), mr));
}

//-----------------------------------------------------------------------------

void Smiley::set_radius(int rr)
{
    Circle::set_radius(rr);
    er = 1.0*rr/delim1;
    mr = 1.0*rr/delim2;
    // Пересчитываем координаты с новым радиусом
    auto eye = get_eye(point(0), radius(), er);
    set_point(1, eye.first);
    set_point(2, eye.second);
    set_point(3, get_mouth(point(0), radius(), mr));
}

//-----------------------------------------------------------------------------

void Smiley::draw_lines() const
{
    Circle::draw_lines();

    if (color().visibility())
        for (int i=1; i < number_of_points(); ++i ) {
            fl_color(color().as_int());
            if (i == number_of_points()-1) // Рисуем улыбку
                fl_arc(point(i).x, point(i).y,
                       mr+mr, mr+mr, 180, 360);
            else                            // Рисуем глаза
                fl_arc(point(i).x, point(i).y,
                       er+er, er+er, 0, 360);
        }
}

//-----------------------------------------------------------------------------

void Smiley_hat::draw_lines() const
{
    Smiley::draw_lines();
    const int& mr = mouth_radius();

    if (fill_color().visibility()) {	// fill
        fl_color(fill_color().as_int());

        fl_rectf(point(0).x,point(0).y-mr,
                 2*radius(), mr);
        fl_pie(point(0).x - 2*mr + radius(), point(0).y-3*mr,
               mr+mr+mr+mr, mr+mr+mr+mr, 0, 180);

        fl_color(color().as_int());	// reset color
    }

    if (color().visibility()) {	// edge on top of fill
        fl_color(color().as_int());

        fl_rect(point(0).x,point(0).y-mr,
                2*radius(), mr);
        fl_arc(point(0).x - 2*mr + radius(), point(0).y-3*mr,
                mr+mr+mr+mr, mr+mr+mr+mr, 0, 180);
    }
}

//-----------------------------------------------------------------------------

void Frowny::draw_lines() const
{
    Circle::draw_lines();
    const int& mr = mouth_radius();
    const int& er = eye_radius();

    if (color().visibility())
        for (int i=1; i < number_of_points(); ++i ) {
            fl_color(color().as_int());
            if (i == number_of_points()-1) // Рисуем frowny рот
                fl_arc(point(i).x, point(i).y,
                       mr+mr, mr+mr, 0, 180);
            else                            // Рисуем глаза
                fl_arc(point(i).x, point(i).y,
                       er+er, er+er, 0, 360);
        }
}

//-----------------------------------------------------------------------------

void Frowny_hat::draw_lines() const
{
    Frowny::draw_lines();
    const int& mr = mouth_radius();

    if (fill_color().visibility()) {	// fill
        fl_color(fill_color().as_int());

        fl_rectf(point(0).x,point(0).y-mr,
                 2*radius(), mr);
        fl_pie(point(0).x - 2*mr + radius(), point(0).y-3*mr,
               mr+mr+mr+mr, mr+mr+mr+mr, 0, 180);

        fl_color(color().as_int());	// reset color
    }

    if (color().visibility()) {	// edge on top of fill
        fl_color(color().as_int());

        fl_rect(point(0).x,point(0).y-mr,
                2*radius(), mr);
        fl_arc(point(0).x - 2*mr + radius(), point(0).y-3*mr,
                mr+mr+mr+mr, mr+mr+mr+mr, 0, 180);
    }
}

//-----------------------------------------------------------------------------

void Ellipse::draw_lines() const
{
	if (fill_color().visibility()) {	// fill
		fl_color(fill_color().as_int());
		fl_pie(point(0).x,point(0).y,w+w-1,h+h-1,0,360);
		fl_color(color().as_int());	// reset color
	}

	if (color().visibility()) {
		fl_color(color().as_int());
        fl_arc(point(0).x,point(0).y,w+w,h+h,0,360);
	}
}

void Arc::draw_lines() const
{
    if (fill_color().visibility()) {	// fill
        fl_color(fill_color().as_int());
        fl_pie(point(0).x,point(0).y,major()+major()-1,minor()+minor()-1,ba,ea);
        fl_color(color().as_int());	// reset color
    }
    if (color().visibility()) {
        fl_color(color().as_int());
        fl_arc(point(0).x, point(0).y, major()+major(),
               minor()+minor(),ba,ea);
    }
}

void draw_mark(Point xy, char c)
{
	static const int dx = 4;
	static const int dy = 4;
	string m(1,c);
	fl_draw(m.c_str(),xy.x-dx,xy.y+dy);
}

void Marked_polyline::draw_lines() const
{
	Open_polyline::draw_lines();
	for (int i=0; i<number_of_points(); ++i)
		draw_mark(point(i),mark[i%mark.size()]);
}

//-----------------------------------------------------------------------------

int get_width_string(const string& s)
{
    int ofsize = fl_size();
    fl_font(fl_font(), 14);
    int width = fl_width(s.c_str());
    width += 20;
    fl_font(fl_font(), ofsize);
    return width;
}

//-----------------------------------------------------------------------------

int get_height_string()
{
    int height = fl_height(fl_font(), 14);
    height += 20;
    return height;
}

//-----------------------------------------------------------------------------

Texted_box::Texted_box(Point xy, const string &s)
    : Box {xy, get_width_string(s), get_height_string()}, lab{s}
{

}

//-----------------------------------------------------------------------------
// Exercise 13_6
void Texted_box::draw_lines() const
{
    Box::draw_lines();
    fl_draw(lab.c_str(),point(0).x+10,point(0).y+20);
}
//-----------------------------------------------------------------------------
/*
void Marks::draw_lines() const
{
	for (int i=0; i<number_of_points(); ++i)
		fl_draw(mark.c_str(),point(i).x-4,point(i).y+4);
}
*/


std::map<string,Suffix::Encoding> suffix_map;

int init_suffix_map()
{
	suffix_map["jpg"] = Suffix::jpg;
	suffix_map["JPG"] = Suffix::jpg;
	suffix_map["jpeg"] = Suffix::jpg;
	suffix_map["JPEG"] = Suffix::jpg;
	suffix_map["gif"] = Suffix::gif;
	suffix_map["GIF"] = Suffix::gif;
	suffix_map["bmp"] = Suffix::bmp;
	suffix_map["BMP"] = Suffix::bmp;
	return 0;
}

Suffix::Encoding get_encoding(const string& s)
		// try to deduce type from file name using a lookup table
{
	static int x = init_suffix_map();

	string::const_iterator p = find(s.begin(),s.end(),'.');
	if (p==s.end()) return Suffix::none;	// no suffix

	string suf(p+1,s.end());
	return suffix_map[suf];
}

bool can_open(const string& s)
            // check if a file named s exists and can be opened for reading
{
	ifstream ff(s.c_str());
	if (!ff) return false;
	return true;
}


// somewhat overelaborate constructor
// because errors related to image files can be such a pain to debug
Image::Image(Point xy, string s, Suffix::Encoding e)
	:w(0), h(0), fn(xy,"")
{
	add(xy);

	if (!can_open(s)) {
		fn.set_label("cannot open \""+s+'\"');
		p = new Bad_image(30,20);	// the "error image"
		return;
	}

	if (e == Suffix::none) e = get_encoding(s);

	switch(e) {
	case Suffix::jpg:
		p = new Fl_JPEG_Image(s.c_str());
		break;
	case Suffix::gif:
		//Fl_GIF_Image fl_gf(s.c_str());
		p = new Fl_GIF_Image(s.c_str());
		break;
//	case Suffix::bmp:
//		p = new Fl_BMP_Image(s.c_str());
//		break;
	default:	// Unsupported image encoding
		fn.set_label("unsupported file type \""+s+'\"');
		p = new Bad_image(30,20);	// the "error image"
	}
}

void Image::draw_lines() const
{
	if (fn.label()!="") fn.draw_lines();

	if (w&&h)
		p->draw(point(0).x,point(0).y,w,h,cx,cy);
	else
		p->draw(point(0).x,point(0).y);
}

//-----------------------------------------------------------------------------

Analog_clock::Analog_clock(Point xy, int rr)
    : dial{xy, rr}
{
    vr.push_back(new Mutable_line());
    vr.push_back(new Mutable_line());
    vr.push_back(new Mutable_line());

    vr[Clock_hand::h].set_style(Line_style{Line_style::solid, 3});
    vr[Clock_hand::m].set_style(Line_style{Line_style::solid, 3});
    update();
}

//-----------------------------------------------------------------------------
// Получаем точку соответствующую
// текущему виду часовой стрелки и ее времени
inline Point get_clock_hand_end(Point o, int r, int common, int current)
{
    double alpha{90 - 360.0 / common * current};
    return Point{static_cast<int>(o.x + r * std::cos(alpha * M_PI / 180.0)),
                 static_cast<int>(o.y - r * std::sin(alpha * M_PI / 180.0))};
}

//-----------------------------------------------------------------------------
// Пересчет координат часовых стрелок
void Analog_clock::update()
{
    constexpr int hours{12};
    constexpr int min_or_sec{60};

    int r{dial.radius()};
    Point o{dial.center()};

    ct.update();    // Обновляем текущее время

    vr[Clock_hand::h].set_point(o, get_clock_hand_end(o, r-30,
                                                      hours, ct.hour()));
    vr[Clock_hand::m].set_point(o, get_clock_hand_end(o, r-10,
                                                      min_or_sec, ct.minute()));
    vr[Clock_hand::s].set_point(o, get_clock_hand_end(o, r-5,
                                                      min_or_sec, ct.second()));
}

//-----------------------------------------------------------------------------

void Analog_clock::set_color(Color c)
{
    dial.set_color(c);
    for (int i=0; i < vr.size(); ++i) vr[i].set_color(c);
}

//-----------------------------------------------------------------------------

void Analog_clock::move(int dx, int dy)
{
    dial.move(dx, dy);
    for (int i=0; i < vr.size(); ++i) vr[i].move(dx, dy);
}

//-----------------------------------------------------------------------------

void Analog_clock::draw_lines() const
{
    dial.draw_lines();
    for (int i=0; i < vr.size(); ++i) vr[i].draw_lines();
}

//-----------------------------------------------------------------------------

} // Graph
