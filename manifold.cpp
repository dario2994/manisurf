#include <type_traits>
#include <iostream>
#include <vector>
#include <fstream>

// Element of the euclidean space. 
// Used to identify an element of the tangent bundle.
// TODO: Choose better name (maybe Vector)
class SpacePoint {
    public:
    double x;
    double y;
    double z;
    
    SpacePoint(double x, double y, double z) : x(x), y(y), z(z) {}
    SpacePoint() : SpacePoint(0, 0, 0) {}
    
    SpacePoint operator *(const double& r) const {
        SpacePoint res = SpacePoint(x, y, z);
        res.x *= r;
        res.y *= r;
        res.z *= r;
        return res;
    }
};

// As we will work only with oriented manifolds, SO3 will always be enough.
// The fact is that the base we will choose on the tangent bundle will be
// orthonormal and positive in each point.
class SO3 {
    public:
    SO3() {}
    static SO3 Identity() {
        return SO3();
    }
    
    // Apply the rotation to the given point of the space.
    SpacePoint Apply(const SpacePoint& point) const {
        return point; // TODO
    }
    
    // Changes this to (other * this).
    void Compose(const SO3& other) {
        // TODO
    }
};

// Only oriented 3-manifolds are supported.
// To work with non oriented manifolds, just consider the proper oriented 
// covering (all manifolds admit an oriented covering of degree 2).
template <class T>
concept bool ManifoldType() {
    return
        requires() {
            // This should init the coordinates to a certain point on the manifold.
            {T()} -> T;
        } &&
        requires(T M, const SpacePoint& v) {
            // What should happen to coordinates and rotation when we advance of r?
            // The vector v can be assumed very small in order to simplify 
            // the calculations.
            // Advancing means going on the geodesic from the actual point and 
            // directing towards v.
            // The coordinates members are modified, the rotation adjustment is 
            // returned.
            {M.ExponentialMap(v)} -> SO3;
        } &&
        requires(const T& M, const T& N) {
            // Where is the point other with respect to our position?
            // The result is an element of R3, that identifies a point on the tangent 
            // bundle of the current point. If we apply the exponential map to this
            // element of the tanget, we would obtain the point 'other'.
            // The smallest such element of the tangent should be returned.
            {M.AntiExponentialMap(N)} -> std::vector<SpacePoint>; 
        };
}

// The surfer handler.
// The state is given by a point on the manifolds and by the current rotation.
// It is possible to go forward for r (meters/millimeters or whatever),
// to rotate the visual and to know where is a given point from the current
// point of view.
// All the "hard computation" are delegated to the Manifold class.
template <ManifoldType Manifold>
class ManifoldSurfer {
    public:
    ManifoldSurfer() {
        point_ = Manifold();
        rotation_ = SO3::Identity();
        speed_ = 0.0;
    }
    
    ManifoldSurfer(Manifold point) {
        point_ = point;
        rotation_ = SO3::Identity();
        speed_ = 0.0;
    }
    
    ManifoldSurfer(Manifold point, SO3 rotation) {
        point_ = point;
        rotation_ = rotation;
        speed_ = 0.0;
    }
    
    ManifoldSurfer(Manifold point, SO3 rotation, double speed) {
        point_ = point;
        rotation_ = rotation;
        speed_ = speed;
    }
    
    ManifoldSurfer(Manifold point, SO3 rotation, double speed, 
                   std::vector<Manifold> objects) {
        point_ = point;
        rotation_ = rotation;
        speed_ = speed;
        objects_ = objects;
    }
    
    // Changes coordinates and rotation as if we were going forward of r.
    // The real r>0 can be assumed very small in order to simplify calculations.
    // Advancing means going on the geodesic from the actual point and directing
    // towards rotation_(∂z).
    void Advance(double r) {
        const static SpacePoint direction = 
                rotation_.Apply(SpacePoint(0, 0, 1));
        rotation_.Compose(point_.ExponentialMap(direction));
    }
    
    // Rotate the vision of the given rotation.
    void RotateVision(const SO3& rotation) {
        rotation_.Compose(rotation);
    }
    
    // Accelerate speed.
    void Accelerate() {
        speed_ += 0.1;
    }
    
    // Decelerate
    void Decelerate() {
        speed_ += 0.1;
        if (speed_ < 0.0) {
            speed_ = 0.0;
        }
    }
    
    // Rotate the camera. It changes rotation_.
    void Rotate() {
        
    }
    
    // This adjust the result of AntiExponential to the current rotation_.
    SpacePoint PointVision(const Manifold& other) const {
        SpacePoint res = point_.AntiExponentialMap(other);
        return rotation_.Apply(res);
    }
    
    private:
    Manifold point_;
    std::vector<Manifold> objects_;
    SO3 rotation_;
    double speed_;
    // Maybe speed?
};

class R3 {
    private:
    // R^3 is canonically parametrized by x, y, z
    // The base of the tangent bundle is the obvious one i.e. ∂x,∂y,∂z.
    double x;
    double y;
    double z;

    public:
    R3(double x, double y, double z) : x(x), y(y), z(z) {}
    R3() : R3(0, 0, 0) {}

    SO3 ExponentialMap(const SpacePoint& v) {
        x += v.x;
        y += v.y;
        z += v.y;
        return SO3::Identity();
    }

    std::vector<SpacePoint> AntiExponentialMap(const R3& other) const {
        SpacePoint result;
        result.x = other.x - x;
        result.y = other.y - y;
        result.z = other.z - z;
        return {result};
    }
};

#include "OpenglHandler.cpp"

int main() {
    OpenglRun();
    
    ManifoldSurfer<R3> surfer;
}
