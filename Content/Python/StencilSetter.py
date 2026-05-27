import unreal

stencil_id = 44
cantidad_total = 0

actor_utils = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
todos_los_actores = actor_utils.get_all_level_actors()

def aplicar_stencil_recursivo(component, actor):
    global cantidad_total
    if not component or not actor:
        return

    if isinstance(component, unreal.StaticMeshComponent):
        if not component.get_editor_property("render_custom_depth") or \
           component.get_editor_property("custom_depth_stencil_value") != stencil_id:

            component.set_editor_property("render_custom_depth", True)
            component.set_render_custom_depth(True)
            component.set_editor_property("custom_depth_stencil_value", stencil_id)

            comp_name = component.get_name()
            actor_name = actor.get_name()
            actor_path = actor.get_path_name()

            unreal.log(f"Stencil {stencil_id} aplicado a componente '{comp_name}' en actor '{actor_name}' ({actor_path})")
            cantidad_total += 1

    # Recorrer hijos (por si están anidados)
    if hasattr(component, "get_children_components"):
        for child in component.get_children_components(True):
            aplicar_stencil_recursivo(child, actor)

def aplicar_stencil_en_actor(actor):
    # Obtiene todos los StaticMeshComponents (independientemente de root)
    static_mesh_components = actor.get_components_by_class(unreal.StaticMeshComponent)

    for comp in static_mesh_components:
        aplicar_stencil_recursivo(comp, actor)

    # Recurse en actores hijos si los hay
    child_components = actor.get_components_by_class(unreal.ChildActorComponent)
    for child_comp in child_components:
        child_actor = child_comp.get_editor_property("child_actor")
        if child_actor:
            aplicar_stencil_en_actor(child_actor)

for actor in todos_los_actores:
    aplicar_stencil_en_actor(actor)

unreal.log(f"✅ Se aplicó Stencil ID {stencil_id} a {cantidad_total} StaticMeshComponents.")