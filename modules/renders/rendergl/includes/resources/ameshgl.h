#ifndef MESHGL_H
#define MESHGL_H

#include <list>

#include <resources/mesh.h>
#include <amath.h>

class CommandBufferGL;

class AMeshGL : public Mesh {
    A_OVERRIDE(AMeshGL, Mesh, Resources)
public:
    typedef vector<IndexVector> BufferVector;

public:
    AMeshGL                     ();

    ~AMeshGL                    ();

    void                        apply               ();

    void                        clear               ();

    void                        bindVao             (CommandBufferGL *buffer, uint32_t surface, uint32_t lod);

    uint32_t                    instance            () const;

protected:
    void                        updateVao           (uint32_t surface, uint32_t lod);

public:
    BufferVector                m_triangles;
    BufferVector                m_uv0;
    BufferVector                m_uv1;
    BufferVector                m_uv2;
    BufferVector                m_uv3;
    BufferVector                m_normals;
    BufferVector                m_tangents;
    BufferVector                m_vertices;
    BufferVector                m_colors;
    BufferVector                m_weights;
    BufferVector                m_indices;

    uint32_t                    m_InstanceBuffer;

    typedef map<CommandBufferGL *, uint32_t>  VaoMap;

    typedef vector<vector<VaoMap>> VaoVector;

    VaoVector                   m_Vao;
};

#endif // MESHGL_H
