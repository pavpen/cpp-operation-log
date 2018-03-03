
R"code(

<style>
    .operation-log-3js-scene-container > canvas {
        width: 500;
        height: 500;
    }

    .operation-log-3js-scene-container {
        position: relative;
    }

    .operation-log-3js-scene-view-label {
        color: white;
        text-shadow: 0 0 3px #FF0000;
        display: block;
    }
</style>

<script type="text/javascript" src="https://threejs.org/build/three.min.js"></script>
<script type="text/javascript" src="https://threejs.org/examples/js/controls/TrackballControls.js"></script>

<script>

var operation_log_3js = (function() {

class ThreeJsDocument
{
    constructor()
    {
        this.renderers = [];

        var self = this;

        this.animate = function animate(timestamp)
        {
            for (var i = 0; i < self.renderers.length; ++i)
            {
                var sceneView = self.renderers[i];

                if (sceneView.animationFrame)
                {
                    sceneView.animationFrame(timestamp, sceneView);
                    continue;
                }
                sceneView.render();
            }

            requestAnimationFrame(self.animate);
        };
    }

    addSceneView(sceneView)
    {
        var scripts = document.getElementsByTagName('script');
        var currentScript = scripts[scripts.length - 1];
        var containerElement = document.createElement('div');

        containerElement.className = sceneView.containerElementClassName;
        containerElement.appendChild(sceneView.renderer.domElement);
        currentScript.parentNode.appendChild(containerElement);
        sceneView.containerElement = containerElement;

        if (sceneView.animationFrame || sceneView.controls)
        {
            this.renderers.push(sceneView);
        }
        if (sceneView.renderer)
        {
            sceneView.renderer.render(sceneView.scene, sceneView.camera);
            for (var label_i = 0; label_i < sceneView.labels.length; ++label_i)
            {
                var label = sceneView.labels[label_i];

                containerElement.appendChild(label.domElement);
                label.updatePosition(
                    sceneView.camera, sceneView.renderer);
            }
        }
    }

    addListeners()
    {
        var self = this;

        window.addEventListener('load', function() {
            self.animate();
        });
    }
}

class SceneView
{
    constructor(scene, renderer, camera)
    {
        this.scene = scene;
        this.renderer = renderer;
        this.camera = camera;
        this.labels = [];
        this.labelClassName = 'operation-log-3js-scene-view-label';
        this.controls = null; // E.g., mouse controls for rotating the view camera.
        this.controlsUpdating = false;
        this.animationFrame = null; // Call back for each scene frame.
        this.containerElement = null; // Set when the view is added to the DOM.
        this.containerElementClassName = 'operation-log-3js-scene-container';
    }

    createLabel(
        labelHtml, renderer,
        position3d, referenceMesh, referenceVertexIndex, transformPosition)
    {
        const domElement = document.createElement('div');

        domElement.className = this.labelClassName;
        domElement.innerHTML = labelHtml;
        domElement.style.position = 'absolute';

        var label;
        
        if (referenceMesh)
        {
            if (typeof referenceVertexIndex == 'number')
            {
                label = new MeshVertexReferencedDomElement(
                    domElement, position3d, referenceMesh, referenceVertexIndex, transformPosition);
            }
            else
            {
                label = new MeshReferencedDomElement(
                    domElement, position3d, referenceMesh, transformPosition);
            }
        }
        else
        {
            label = new SceneReferencedDomElement(domElement, position3d);
        }
        return label;
    }

    labelVertices(mesh, labelHtmlLambda)
    {
        const vertex_ofs = new THREE.Vector3(0, 0, 0);

        for (var v_i = 0; v_i < mesh.geometry.vertices.length; ++v_i)
        {
            var label;
            
            if (labelHtmlLambda)
            {
                label = this.createLabel(
                    labelHtmlLambda(v_i), this.renderer, vertex_ofs, mesh, v_i, true);
            }
            else
            {
                label = this.createLabel(
                    'v<sub>' + v_i + '</sub>', this.renderer, vertex_ofs, mesh, v_i, true);
            }
            this.labels.push(label);
        }
    }

    render()
    {
        if (this.controls && !this.controlsUpdating)
        {
            this.controlsUpdating = true;
            this.controls.update();
            this.controlsUpdating = false;
        }
        this.renderer.render(this.scene, this.camera);
        if (this.labels)
        {
            for (var label_i = 0; label_i < this.labels.length; ++label_i)
            {
                var label = this.labels[label_i];

                label.updatePosition(this.camera, this.renderer);
            }
        }
    }
}

class SceneReferencedDomElement
{
    constructor(domElement, position3d)
    {
        this.domElement = domElement;
        this.position3d = position3d;
        if (!this.position3d)
        {
            this.position3d = new THREE.Vector3(0, 0, 0);
        }
        this.position3dBuf = this.position3d;
    }

    updatePosition(camera, renderer)
    {
        var position2d = this.position3dBuf.project(camera);

        position2d.x = (1 + position2d.x) / 2 * renderer.domElement.clientWidth;
        position2d.y = (1 - position2d.y) / 2 * renderer.domElement.clientHeight;

        this.domElement.style.left = position2d.x + 'px';
        this.domElement.style.top = position2d.y + 'px';
    }
}

class MeshReferencedDomElement extends SceneReferencedDomElement
{
    constructor(domElement, position3d, referenceMesh, transformPosition)
    {
        super(domElement, position3d);
        this.domElement = domElement;
        this.referenceMesh = referenceMesh;
        this.transformPosition = transformPosition;
        this.position3dBuf = new THREE.Vector3(0, 0, 0);
    }

    updatePosition(camera, renderer)
    {
        if (this.transformPosition)
        {
            this.position3dBuf.copy(this.position3d);
            this.referenceMesh.updateMatrixWorld();
            this.position3dBuf.applyMatrix4(this.referenceMesh.matrixWorld);
        }
        else
        {
            this.position3dBuf.copy(this.position3d);
            this.position3dBuf.add(this.referenceMesh.position);
        }
        super.updatePosition(camera, renderer);
    }
}

class MeshVertexReferencedDomElement extends SceneReferencedDomElement
{
    // @transformPosition: Whether @position3d is in @referenceMesh's local reference frame (or the world frame).
    constructor(domElement, position3d, referenceMesh, referenceVertexIndex, transformPosition)
    {
        super(domElement, position3d);
        this.domElement = domElement;
        this.referenceMesh = referenceMesh;
        this.referenceVertexIndex = referenceVertexIndex;
        this.transformPosition = transformPosition;
        this.position3dBuf = new THREE.Vector3(0, 0, 0);
    }

    updatePosition(camera, renderer)
    {
        if (this.transformPosition)
        {
            this.position3dBuf.copy(
                this.referenceMesh.geometry.vertices[this.referenceVertexIndex]);
            this.position3dBuf.add(this.position3d);
            this.referenceMesh.updateMatrixWorld();
            this.position3dBuf.applyMatrix4(this.referenceMesh.matrixWorld);
        }
        else
        {
            this.position3dBuf.copy(
                this.referenceMesh.geometry.vertices[this.referenceVertexIndex]);
            this.referenceMesh.updateMatrixWorld();
            this.position3dBuf.applyMatrix4(this.referenceMesh.matrixWorld);
            this.position3dBuf.add(this.position3d);
        }
        super.updatePosition(camera, renderer);
    }
}

const l3js_document = new ThreeJsDocument();

l3js_document.addListeners();

return {
    document: l3js_document,
    SceneView: SceneView
};

})();

</script>

)code"