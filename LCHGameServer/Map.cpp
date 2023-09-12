#include "pch.h"
#include "Map.h"
#include "ZoneManager.h"

void Map::LoadMap(int mapId, std::string pathPrefix)
{
    std::string mapName = "/Map_" + std::to_string(mapId) + ".txt";
    std::ifstream openFile(pathPrefix + mapName);
    CRASH_ASSERT(openFile.is_open());

    std::string line;
    getline(openFile, line);
    MinX = stoi(line);

	getline(openFile, line);
	MaxX = stoi(line);

	getline(openFile, line);
	MinY = stoi(line);

	getline(openFile, line);
	MaxY = stoi(line);

	xCount = MaxX - MinX + 1;
	yCount = MaxY - MinY + 1;

    for (uint32 y = 0; y < yCount; y++)
    {
        std::string mapLine;
        getline(openFile, mapLine);
        for (uint32 x = 0; x < xCount; x++)
        {
            CollisionBuf[y][x] = (mapLine[x] == '1' ? true : false);
        }
    }

    openFile.close();
}

bool Map::CanGo(Vector2Int cellPos, bool checkActor)
{
    if (cellPos.x < MinX || cellPos.x > MaxX)
        return false;
    if (cellPos.y < MinY || cellPos.y > MaxY)
        return false;

    int x = cellPos.x - MinX;
    int y = MaxY - cellPos.y;
    return !CollisionBuf[y][x] && (!checkActor || ObjectBuf[y][x] == nullptr);
}

ActorPtr Map::Find(Vector2Int cellPos)
{
    if (cellPos.x < MinX || cellPos.x > MaxX)
        return nullptr;
    if (cellPos.y < MinY || cellPos.y > MaxY)
        return nullptr;

	int32 x = cellPos.x - MinX;
    int32 y = cellPos.y - MinY;
    return ObjectBuf[y][x];
}

bool Map::ApplyLeave(ActorPtr actor)
{
    auto _zone = GZoneManager.FindZoneByID(actor->zoneID);
    if (_zone == nullptr)
        return false;
    
	protocol::PositionInfo posInfo = actor->ActorInfo.posinfo();
	if (posInfo.posx() < MinX || posInfo.posx() > MaxX)
		return false;
	if (posInfo.posy() < MinY || posInfo.posy() > MaxY)
		return false;

	{
		int x = posInfo.posx() - MinX;
		int y = MaxY - posInfo.posy();
		if (ObjectBuf[y][x] == actor)
            ObjectBuf[y][x] = nullptr;

	}
    return true;
}

bool Map::ApplyMove(ActorPtr actor, Vector2Int dest)
{
    ApplyLeave(actor);
    auto _zone = GZoneManager.FindZoneByID(actor->zoneID);
    if (_zone == nullptr)
        return false;

    protocol::PositionInfo posInfo = actor->ActorInfo.posinfo();
    if (CanGo(dest, true) == false)
        return false;

    {
        int32 x = dest.x - MinX;
        int32 y = MaxY - dest.y;
        ObjectBuf[y][x] = actor;
    }

    posInfo.set_posx(dest.x);
    posInfo.set_posy(dest.y);
    return true;
}

std::list<Vector2Int> Map::FindPath(Vector2Int startCellPos, Vector2Int destCellPos, bool CheckActor)
{
	int32 _deltaY[] = { 1, -1, 0, 0 };
	int32 _deltaX[] = { 0, 0, -1, 1 };
	int32 _cost[]   = { 10, 10, 10, 10 };

    std::vector<Pos> path;

    bool closed[MAX_MAP_SIZE][MAX_MAP_SIZE];
    memset(closed, false, sizeof(closed));

    int32 opened[MAX_MAP_SIZE][MAX_MAP_SIZE];
    for (int32 y = 0; y < SizeY; y++)
    {
        for (int32 x = 0; x < SizeX; x++)
        {
            opened[y][x] = INT_MAX;
        }
    }

    Pos parent[MAX_MAP_SIZE][MAX_MAP_SIZE];

    std::priority_queue<PQNode, std::vector<PQNode>, std::greater<PQNode>> pq;

    Pos pos = Cell2Pos(startCellPos);
    Pos dest = Cell2Pos(destCellPos);

    int32 g = 0;
    int32 h = 10 * (abs(dest.y - pos.y) + abs(dest.x - pos.x));
    opened[pos.y][pos.x] = h;
    pq.push(PQNode{ g + h, g, pos });
    parent[pos.y][pos.x] = Pos(pos.y, pos.x);

    while (pq.size() > 0)
    {
        PQNode node = pq.top();
        pq.pop();
        if(closed[node.pos.y][node.pos.x] == true)
            continue;

        closed[node.pos.y][node.pos.x] = true;
        if(node.pos.y == dest.y && node.pos.x == dest.x)
            break;

        for (uint32 i = 0; i < ARRAY_CNT(_deltaY); i++)
        {
            Pos next = Pos(node.pos.y + _deltaY[i], node.pos.x + _deltaX[i]);
            if (next.y != dest.y || next.x != dest.x)
            {
                if(CanGo(Pos2Cell(next), CheckActor) == false)
                    continue;
            }

            if(closed[next.y][next.x])
                continue;

            g = 0;
            h = 10 * ((dest.y - next.y) * (dest.y - next.y) + (dest.x - next.x) * (dest.x - next.x));
            if(opened[next.y][next.y] < g + h)
                continue;

            opened[dest.y][dest.x] = g + h;
            pq.push(PQNode{ g + h, g, next });
            parent[next.y][next.x] = Pos(node.pos.y, node.pos.x);
        }
    }

    return CalcCellPathFromParent(parent, dest);
}

std::list<Vector2Int> Map::CalcCellPathFromParent(Pos parent[MAX_MAP_SIZE][MAX_MAP_SIZE], Pos dest)
{
    std::list<Vector2Int> cells = std::list<Vector2Int>();
    int32 y = dest.y;
    int32 x = dest.x;
    while (parent[y][x].y != y || parent[y][x].x != x)
    {
        cells.push_back(Pos2Cell(Pos(y, x)));
        Pos pos = parent[y][x];
        y = pos.y;
        x = pos.x;
    }
    cells.push_back(Pos2Cell(Pos(y, x)));
    cells.reverse();
    return cells;
}

Pos Map::Cell2Pos(Vector2Int cell)
{
    return Pos(MaxY - cell.y, cell.x - MinX);
}

Vector2Int Map::Pos2Cell(Pos pos)
{
    return Vector2Int(pos.x + MinX, MaxY - pos.y);
}