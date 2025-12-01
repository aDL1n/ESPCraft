namespace sgl 
{
    class Vec2
    {
    public:
        float x, y;

        Vec2() : x(0), y(0)
        {
        }

        Vec2(float x, float y) : x(x), y(y)
        {
        }
    };

    class IVec2
    {
    public:
        int x, y;

        IVec2() : x(0), y(0)
        {
        }

        IVec2(int x, int y) : x(x), y(y)
        {
        }
    };
}