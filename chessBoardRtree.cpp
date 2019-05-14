#include "chessBoardRtree.h"

ChessBoardRtree::ChessBoardRtree(const std::vector<std::vector<Point2f>> & points):ChessBoard(points){

}

void ChessBoardRtree::setValue(const Point2i index, float value){
	Point2f corner = corners[index.x][index.y];

	float x1, y1, x2, y2;
	if(index.x != 0){
		x1 = corners[index.x - 1][index.y].x;
	}else{
		x1 = 0;
	}

	if(index.y != 0){
		y1 = corners[index.x][index.y - 1].y;
	}else{
		y1 = 0;
	}

	if(index.x != size.width){
		x2 = corners[index.x + 1][index.y].x;
	}else{
		x2 = resolution.width;
	}

	if(index.y != size.height){
		y2 = corners[index.x][index.y + 1].x;
	}else{
		y2 = resolution.height;
	}

	box b(point((corner.x+x1)/2, (corner.y+y1)/2), point((corner.x+x2)/2, (corner.y+y2)/2));

    rtree.insert(std::make_pair(b, value));
    indexRtree.insert(std::make_pair(b, index));
}

Point2i ChessBoardRtree::nearest(const Point2f p) const{
	std::vector<IndexValue> result;
	indexRtree.query(bgi::nearest(point(p.x, p.y), 1), std::back_inserter(result));
	return result.at(0).second;
}

float ChessBoardRtree::getValue(const Point2f p) const{
	std::vector<value> result;
		rtree.query(bgi::nearest(point(p.x, p.y), 1), std::back_inserter(result));
		return result.at(0).second;
}
