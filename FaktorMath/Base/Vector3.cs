namespace FaktorMath.Base
{
    public class Vector3 
    {
        public float X;
        public float Y;
        public float Z;
        public float Length => Lenth(X,Y,Z);

        public Vector3(float x, float y, float z)
        {
            this.X = x;
            this.Y = y;
            this.Z = z;
        }

        public static float Lenth(float x, float y, float z) => MathF.Sqrt(x * x + y * y + z * z);
        public static Vector3 VectorBetweenPoints(Point3 a, Point3 b) => new(b.X - a.X, b.Y - a.Y, b.Z - a.Z);
        public static Vector3 VectorAdd(Vector3 a, Vector3 b) => new(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        public static Vector3 VectorSubtract(Vector3 a, Vector3 b) => new(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
        public static float ScalarProductOfVectors(Vector3 a, Vector3 b) => a.X * b.X + a.Y * b.Y + a.Z * b.Z;
        public static float Angle(Vector3 a, Vector3 b) => MathF.Acos(ScalarProductOfVectors(a, b) / (a.Length * b.Length));
        public static Vector3 Cross(Vector3 a, Vector3 b) => new(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);

        public static float MixedProduct(Vector3 a, Vector3 b, Vector3 c)
        {
            return a.X * (b.Y * c.Z - b.Z * c.Y)
                 - a.Y * (b.X * c.Z - b.Z * c.X)
                 + a.Z * (b.X * c.Y - b.Y * c.X);
        }

        public static Vector3 Projection(Vector3 a, Vector3 b)
        {
            float dot = a.X * b.X + a.Y * b.Y + a.Z * b.Z; // скалярний добуток
            float lenSq = b.X * b.X + b.Y * b.Y + b.Z * b.Z; // |b|²
            float scalar = dot / lenSq; // коефіцієнт масштабування
            return new Vector3(b.X * scalar, b.Y * scalar, b.Z * scalar);
        }

        public static float ScalarProjection(Vector3 a, Vector3 b)
        {
            float dot = a.X * b.X + a.Y * b.Y + a.Z * b.Z;
            float lenB = MathF.Sqrt(b.X * b.X + b.Y * b.Y + b.Z * b.Z);
            return dot / lenB;
        }
    }
}
