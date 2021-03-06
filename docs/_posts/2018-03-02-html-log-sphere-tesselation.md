---
layout: post
title:  "An Example HTML Log"
date:   2018-03-02 1:17:00
categories: examples html-log
---

## An Example Use of the HTML Formatter

This is a rather long example which logs the steps of a simplistic sphere
tesselation algorithm.  Each time a new vertex is added to the tesselation a
3D scene is added to the log to visualize the step of the algorithm.


### The Log

The output log for dividing each great circle in 3 parts is
[here](../examples/html-log-sphere-tesselation/operation-log.html).

<iframe width="800" height="500" src="../examples/html-log-sphere-tesselation/operation-log.html"></iframe>


### Log Configuration and Logging Functions

{% highlight c++ %}
#include <fstream>
#include <stack>

//
// Operation Log configuration:
//
#define OPERATION_LOG_INIT_FUNCTION_NAMESPACE  output_sphere_config
#define OPERATION_LOG_INIT_FUNCTION_NAME       operation_log_init

#include <operation_log.h>

namespace output_sphere_config
{

void operation_log_init(operation_log::DefaultOperationLog &log)
{
    static std::ofstream output_stream("operation-log.html");
    static operation_log::HtmlFormatter formatter(output_stream, "output_sphere");

    // Load THREE.js:
    formatter.extra_header_code =
        operation_log::HtmlFormatter::three_js_header_code;

    log.set_formatter(formatter);
}

}


//
// Functions for logging CGAL vertices as THREE.js scenes:
//

#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Shape_detection_3/Shape_base.h>

#include "reference_frame.h"

namespace output_sphere_log
{

//
// Render a given number of vertices from a CGAL polyhedron incremental
// builder as a point cloud scene.
//
// @builder: The incremental builder to read vertices from.
// @vertex_count: The number of vertices to read.
// @extra_code: Extra Java Script code to add to the end of the scene.
//
template <class HDS>
void log_polyhedron_builder_vertices(
    CGAL::Polyhedron_incremental_builder_3<HDS> &builder, int vertex_count,
    std::string extra_code = "")
{
    std::stringstream code;

    code << R"code(
<script type="text/javascript">

(function ()
{
    var camera = new THREE.PerspectiveCamera(70, 500 / 500, 0.01, 1000);

    camera.position.z = 50;

    var scene = new THREE.Scene();

    // Show the x, y and z axes in the secene:
    var axesHelper = new THREE.AxesHelper( 20 );
    scene.add( axesHelper );

    // Create the point cloud:
    var material = new THREE.PointsMaterial({ size: 2, vertexColors: THREE.VertexColors });
    var geometry = new THREE.Geometry();
    var colors = [];

    addVertex = function(x, y, z)
    {
        geometry.vertices.push(new THREE.Vector3(x, y, z));
        colors.push(new THREE.Color(0.15, 0.15, 0.85));
    };
)code";

    for (int i = 0; i < vertex_count; ++i)
    {
        typename HDS::Vertex_handle vertex = builder.vertex(i);

        code << "    addVertex(" <<
            vertex->point().x() << ", " <<
            vertex->point().y() << ", " <<
            vertex->point().z() << ");" << std::endl;
    }

        code <<
R"code(

    geometry.colors = colors;
    geometry.computeBoundingBox();

    var pointCloud = new THREE.Points(geometry, material);

    scene.add(pointCloud);

    // Book keeping for rendering the scene:
    var renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(250, 250);
    renderer.autoClear = false;

    var sceneView = new operation_log_3js.SceneView(scene, renderer, camera);

    // Add a label with the vertex index to each vertex:
    sceneView.labelVertices(pointCloud);

    // Add mouse controls for rotating the scene camera:
    var controls = new THREE.TrackballControls( camera );

    controls.rotateSpeed = 1.0;
    controls.zoomSpeed = 1.2;
    controls.panSpeed = 0.8;
    controls.noZoom = false;
    controls.noPan = false;
    controls.staticMoving = true;
    controls.dynamicDampingFactor = 0.3;
    controls.keys = [ 65, 83, 68 ];
    controls.addEventListener('change', function()
        {
            sceneView.render();
        });
    
    sceneView.controls = controls;

    sceneView.animationFrame = function(time) {
            sceneView.render();
        };
)code" <<
        extra_code <<
R"code(
    operation_log_3js.document.addSceneView(sceneView);
})();

</script>    
)code";

    operation_log::OperationLogInstance::get().write_html(code.str());
}


//
// Render a given number of vertices from a spherical tesselation as a point
// cloud scene including the sphere being tesselated, and latitude lines for
// the vertices.
//
template <class HDS>
void log_sphere_tessalation_builder_vertices(
    double circumsphere_r, double latitude_step, double max_latitude,
    CGAL::Polyhedron_incremental_builder_3<HDS> &builder, int vertex_count,
    std::string extra_code = "")
{
    std::stringstream extra_code_buf(extra_code);

    // Add the sphere being tessalated:
    extra_code_buf << R"code(
    geometry = new THREE.SphereGeometry(
)code" << circumsphere_r << R"code(, 32, 32);
    material = new THREE.MeshBasicMaterial( { color: 0xdcb856, transparent: true, opacity: 0.5 } );
    var mesh = new THREE.Mesh(geometry, material);

    scene.add(mesh);
)code";

    // Add latitude lines:
    for (double latitude = -CGAL_M_PI_2 + latitude_step;
        latitude <= max_latitude;
        latitude += latitude_step)
    {
        extra_code_buf << R"code(

    geometry = new THREE.CircleGeometry()code" << circumsphere_r * cos(latitude) + 0.1 << R"code(, 32 );
    geometry.translate(0, 0, )code" << circumsphere_r * sin(latitude) << R"code();
    geometry.vertices.shift();
    geometry.vertices.push(geometry.vertices[0]);
    material = new THREE.LineDashedMaterial( {
            color: 0xffffff,
            linewidth: 1,
            dashSize: 4,
            gapSize: 1,
        } );
    mesh = new THREE.Line( geometry, material );
    mesh.computeLineDistances();
    scene.add( mesh );

)code";
    }

    log_polyhedron_builder_vertices(builder, vertex_count, extra_code_buf.str());
}

}
{% endhighlight %}


### The Tesselation Builder Class

{% highlight c++ %}
#include <operation_log.h>


namespace cpp_cad
{

// A class that uses a polyhedron incremental builer to build the faces of a
// sphere tessalation.
template <class HDS>
class Sphere_3_TessalationBuilder : public CGAL::Modifier_base<HDS>
{
private:
    Kernel::FT circumsphere_r;
    int linear_subdivisions;
    CGAL::Polyhedron_3<Kernel> polyhedron;
    int vertex_count;
    CGAL::Polyhedron_incremental_builder_3<HDS> builder;
    double latitude;
    double latitude_step;
    double longitude;
    double longitude_step;
    double prev_longitude;
    double prev_longitude_step;
    double parallel_r; // Radius of the current parallel circle.
    int half_meridian_subdivision_c;
    int parallel_subdivision_c;
    int prev_parallel_subdivision_c;
    int parallel_vertex_i;
    int prev_parallel_vertex_i;
    int parallel_last_vertex_i;
    int prev_parallel_last_vertex_i;

    // Difference between the previous parallel vertex's longitude and the
    // current parallel vertex's longitude in subdivision counts, where the
    // subdivision denominator is
    // prev_parallel_subdivision_c * parallel_subdivision_c.
    int longitude_difference_subdiv;

public:
    inline Sphere_3_TessalationBuilder(CGAL::Polyhedron_3<Kernel> &polyhedron, HDS& hds, Kernel::FT circumsphere_r = 1, int linear_subdivisions = 2)
    : circumsphere_r(circumsphere_r),
        linear_subdivisions(linear_subdivisions),
        polyhedron(polyhedron),
        vertex_count(0),
        CGAL::Modifier_base<HDS>(),
        builder(hds, true)
    {}

    // Required when deriving from CGAL::Modifier_base<HDS> to make this class
    // not abstract:
    void operator()(HDS& hds)
    {}

    void run()
    {
        // First parallel (pole):
        int vertex_count = 1;
        // We over-count the faces by 2, since we count previous parallel
        // vertex count + current parallel vertex count number of faces for
        // each parallel.  However, the poles have one vertex, but need 0
        // triangles to complete the path on the pole parallel (i.e., the
        // 0-radius circle from the pole to itself).
        int face_count = -2;
        int prev_vertex_c = 1;

        // Subdivide a half meridian in, at least, 3 parts:
        half_meridian_subdivision_c =
            std::max(3, (linear_subdivisions + 1) / 2);

        latitude_step = M_PI / half_meridian_subdivision_c;
        double latitude = -CGAL_M_PI_2 + latitude_step;

        for (int parallel_c = 2;
            parallel_c <= half_meridian_subdivision_c;
            ++parallel_c, latitude += latitude_step)
        {
            // Subdivide each parallel into, at least, 3 parts:
            parallel_subdivision_c = std::max(
                3,
                static_cast<int>(ceil(cos(latitude) * linear_subdivisions)));
            int vertex_c = parallel_subdivision_c;

            vertex_count += vertex_c;
            face_count += prev_vertex_c + vertex_c;

            prev_vertex_c = vertex_c;
        }

        // Last parallel (pole):
        face_count += prev_vertex_c;
        ++vertex_count;

        // Each face has 3 halfedges.
        int halfedge_count = 3 * face_count;

        builder.begin_surface(vertex_count, face_count, halfedge_count);
        add_tessalation();
        builder.end_surface();
    }

private:
    void add_tessalation()
    {
        OPERATION_LOG_ENTER_NO_ARG_FUNCTION();

        longitude_step = 2 * M_PI / parallel_subdivision_c;

        latitude = -CGAL_M_PI_2;
        parallel_r = 0.0;
        prev_parallel_vertex_i = 0;
        add_first_parallel();

        latitude += latitude_step;
        parallel_r = circumsphere_r * cos(latitude);
        prev_parallel_subdivision_c = parallel_subdivision_c;
        prev_parallel_last_vertex_i = parallel_last_vertex_i;
        add_second_parallel();

        // Add the remaining parallel circles, except the last pole:
        latitude += latitude_step;
        for (int parallel_c = 3;
            parallel_c <= half_meridian_subdivision_c;
            ++parallel_c, latitude += latitude_step)
        {
            parallel_r = circumsphere_r * cos(latitude);
            prev_longitude = 0;
            prev_parallel_subdivision_c = parallel_subdivision_c;
            prev_longitude_step = longitude_step;
            prev_parallel_last_vertex_i = parallel_last_vertex_i;

            add_parallel();
        }

        parallel_r = 0.0;
        prev_longitude = 0;
        prev_parallel_subdivision_c = parallel_subdivision_c;
        prev_longitude_step = longitude_step;
        prev_parallel_last_vertex_i = parallel_last_vertex_i;
        add_last_parallel();

        OPERATION_LOG_LEAVE_FUNCTION();
    }

    // Adds the first parallel (radius = 0):
    inline void add_first_parallel()
    {
        OPERATION_LOG_ENTER_NO_ARG_FUNCTION();

        add_vertex(-CGAL_M_PI_2, 0);
        parallel_subdivision_c = 1;
        parallel_last_vertex_i = 0;
        parallel_vertex_i = 1;

        OPERATION_LOG_LEAVE_FUNCTION();
    }

    // Adds the second parallel:
    inline void add_second_parallel()
    {
        OPERATION_LOG_ENTER_NO_ARG_FUNCTION();

        // Subdivide each parallel into, at least, 3 parts:
        parallel_subdivision_c = std::max(
            3, static_cast<int>(ceil(cos(latitude) * linear_subdivisions)));
        longitude_step = 2*M_PI / parallel_subdivision_c;
        parallel_last_vertex_i = parallel_vertex_i + parallel_subdivision_c - 1;

        // Add the first vertex for the current parallel:
        add_vertex(latitude, 0);
        ++parallel_vertex_i;

        for (longitude = longitude_step;
            parallel_vertex_i <= parallel_last_vertex_i;
            ++parallel_vertex_i, longitude += longitude_step)
        {
            add_vertex(latitude, longitude);

            add_face(
                prev_parallel_vertex_i,
                parallel_vertex_i - 1,
                parallel_vertex_i);
        }

        // Add the last face:
        add_face(
            prev_parallel_vertex_i,
            parallel_vertex_i - 1,
            parallel_vertex_i - parallel_subdivision_c);

        ++prev_parallel_vertex_i;

        OPERATION_LOG_LEAVE_FUNCTION();
    }

    inline void add_last_parallel()
    {
        OPERATION_LOG_ENTER_NO_ARG_FUNCTION();

        parallel_subdivision_c = 1;
        parallel_last_vertex_i = parallel_vertex_i + parallel_subdivision_c - 1;

        // Add the pole:
        add_vertex(CGAL_M_PI_2, 0);

        // Add the faces:
        for (++prev_parallel_vertex_i;
            prev_parallel_vertex_i <= prev_parallel_last_vertex_i;
            ++prev_parallel_vertex_i)
        {
            add_face(
                prev_parallel_vertex_i - 1,
                parallel_vertex_i,
                prev_parallel_vertex_i);
        }

        // Add the last face:
        add_face(
            prev_parallel_vertex_i - 1,
            parallel_vertex_i,
            prev_parallel_vertex_i - prev_parallel_subdivision_c);

        OPERATION_LOG_LEAVE_FUNCTION();
    }

    inline void add_parallel()
    {
        OPERATION_LOG_ENTER_NO_ARG_FUNCTION();

        // Subdivide each parallel into, at least, 3 parts:
        parallel_subdivision_c = std::max(
            3, static_cast<int>(ceil(cos(latitude) * linear_subdivisions)));
        longitude_step = 2*M_PI / parallel_subdivision_c;
        parallel_last_vertex_i = parallel_vertex_i + parallel_subdivision_c - 1;

        OPERATION_LOG_DUMP_VARS(latitude, parallel_subdivision_c,
            longitude_step, prev_parallel_vertex_i, parallel_vertex_i,
            prev_parallel_last_vertex_i, parallel_last_vertex_i);

        // Add the first vertex for the current longitude circle:
        add_vertex(latitude, 0);
        longitude = longitude_step;

        longitude_difference_subdiv = 0;

        while (true)
        {
            // Advance to the next vertex on the previous parallel
            // (longitude_difference_subdiv += parallel_subdivision_c), or
            // advance to the next vertex on the current parallel
            // (longitude_difference_subdiv -= prev_parallel_subdivision_c):
            if (abs(longitude_difference_subdiv + parallel_subdivision_c) <
            abs(longitude_difference_subdiv - prev_parallel_subdivision_c))
            {
                advance_prev_parallel_vertex();
                if (prev_parallel_vertex_i > prev_parallel_last_vertex_i)
                {
                    complete_parallel();
                    break;
                }
            }
            else
            {
                advance_parallel_vertex();
                if (parallel_vertex_i > parallel_last_vertex_i)
                {
                    complete_prev_parallel();
                    break;
                }
            }
        }

        OPERATION_LOG_LEAVE_FUNCTION();
    }

    // Add a face that connects the current and next vertices on the
    // previous parallel to the current vertex on the current parallel.
    inline void advance_prev_parallel_vertex()
    {
        OPERATION_LOG_ENTER_NO_ARG_FUNCTION();

        int res_next_vertex_i = prev_parallel_vertex_i + 1;
        int next_vertex_i = res_next_vertex_i;

        if (res_next_vertex_i > prev_parallel_last_vertex_i)
        {
            next_vertex_i -= prev_parallel_subdivision_c;
        }

        assert(next_vertex_i != prev_parallel_vertex_i);
        add_face(prev_parallel_vertex_i, parallel_vertex_i, next_vertex_i);

        prev_parallel_vertex_i = res_next_vertex_i;
        longitude_difference_subdiv += parallel_subdivision_c;

        OPERATION_LOG_DUMP_VARS(prev_parallel_vertex_i, longitude_difference_subdiv);
        OPERATION_LOG_LEAVE_FUNCTION();
    }

    // Add a face that connects the current and next vertices on the
    // current parallel to the current vertex on the previous parallel.
    inline void advance_parallel_vertex()
    {
        OPERATION_LOG_ENTER_NO_ARG_FUNCTION();

        int res_next_vertex_i = parallel_vertex_i + 1;
        int next_vertex_i = res_next_vertex_i;

        if (res_next_vertex_i > parallel_last_vertex_i)
        {
            next_vertex_i -= parallel_subdivision_c;
        }
        else
        {
            OPERATION_LOG_DUMP_VARS(latitude, longitude, longitude_step);

            add_vertex(latitude, longitude);
            longitude += longitude_step;
        }

        assert(next_vertex_i != parallel_vertex_i);
        add_face(prev_parallel_vertex_i, parallel_vertex_i, next_vertex_i);

        parallel_vertex_i = res_next_vertex_i;
        longitude_difference_subdiv -= prev_parallel_subdivision_c;

        OPERATION_LOG_DUMP_VARS(parallel_vertex_i, longitude_difference_subdiv);
        OPERATION_LOG_LEAVE_FUNCTION();
    }

    // Connect all remaining vertices on the previous parallel to the current
    // vertex on the current parallel by forming traiangles.
    inline void complete_prev_parallel()
    {
        OPERATION_LOG_ENTER_NO_ARG_FUNCTION();

        assert(prev_parallel_subdivision_c > 2);

        int prev_vertex_i = prev_parallel_vertex_i;
        int parallel_vertex_i = this->parallel_vertex_i;

        if (parallel_vertex_i > parallel_last_vertex_i)
        {
            parallel_vertex_i -= parallel_subdivision_c;
        }

        OPERATION_LOG_DUMP_VARS(prev_parallel_vertex_i);

        while (prev_parallel_vertex_i <= prev_parallel_last_vertex_i)
        {
            ++prev_parallel_vertex_i;
            int next_vertex_i = prev_parallel_vertex_i;

            if (next_vertex_i > prev_parallel_last_vertex_i)
            {
                next_vertex_i -= prev_parallel_subdivision_c;
            }

            OPERATION_LOG_DUMP_VARS(prev_vertex_i, parallel_vertex_i, prev_parallel_vertex_i);

            add_face(prev_vertex_i, parallel_vertex_i, next_vertex_i);

            prev_vertex_i = prev_parallel_vertex_i;
        }

        OPERATION_LOG_LEAVE_FUNCTION();
    }

    // Connect all remaining vertices on the current parallel to the current
    // vertex on the previous parallel by forming traiangles.
    inline void complete_parallel()
    {
        OPERATION_LOG_ENTER_NO_ARG_FUNCTION();

        assert(parallel_subdivision_c > 2);

        int prev_vertex_i = parallel_vertex_i;
        int prev_parallel_vertex_i = this->prev_parallel_vertex_i;

        if (prev_parallel_vertex_i > prev_parallel_last_vertex_i)
        {
            prev_parallel_vertex_i -= prev_parallel_subdivision_c;
        }

        OPERATION_LOG_DUMP_VARS(parallel_vertex_i);

        while (parallel_vertex_i <= parallel_last_vertex_i)
        {
            ++parallel_vertex_i;
            int next_vertex_i = parallel_vertex_i;

            OPERATION_LOG_DUMP_VARS(prev_vertex_i, parallel_vertex_i, prev_parallel_vertex_i);

            if (next_vertex_i > parallel_last_vertex_i)
            {
                next_vertex_i -= parallel_subdivision_c;
            }
            else
            {
                OPERATION_LOG_MESSAGE("Adding next vertex.");

                add_vertex(latitude, longitude);
                longitude += longitude_step;
            }

            add_face(prev_vertex_i, next_vertex_i, prev_parallel_vertex_i);

            prev_vertex_i = parallel_vertex_i;
        }

        OPERATION_LOG_LEAVE_FUNCTION();
    }

    // Adds a triangusar face to the polyhedron.
    //     The vertices must have already been added.
    inline void add_face(int v0_index, int v1_index, int v2_index)
    {
        OPERATION_LOG_ENTER_FUNCTION(v0_index, v1_index, v2_index);

        builder.begin_facet();
        builder.add_vertex_to_facet(v0_index);
        builder.add_vertex_to_facet(v1_index);
        builder.add_vertex_to_facet(v2_index);
        builder.end_facet();

        OPERATION_LOG_LEAVE_FUNCTION();
    }

    inline void add_vertex(double latitude, double longitude)
    {
        OPERATION_LOG_ENTER_FUNCTION(latitude / M_PI, longitude / M_PI);

        assert(abs(parallel_r - circumsphere_r * cos(latitude)) < 1e-15);

        Kernel::Point_3 point(
                parallel_r * cos(longitude),
                parallel_r * sin(longitude),
                circumsphere_r * sin(latitude)
            );

        OPERATION_LOG_MESSAGE_STREAM(<<
            "Vertex " << vertex_count << ": " << point);

        builder.add_vertex(point);
        vertex_count++;

        cpp_cad_log::log_sphere_tessalation_builder_vertices(
            circumsphere_r, latitude_step, latitude, builder, vertex_count);

        OPERATION_LOG_LEAVE_FUNCTION();
    }
};

}
{% endhighlight %}
