#ifndef _INSG_EDITOR_H_
#define _INSG_EDITOR_H_

#include"model_editor_ctrl.h"
#include"array_ctrl.h"
#include"primitive_ctrl.h"
#include"pcs_file.h"

#define NUM_VERTS 3

struct pcs_insig_face_vert {
	vector3d vert;
	float u, v;
	pcs_insig_face_vert() : u(0.0f), v(0.0f) {}
	pcs_insig_face_vert(pcs_insig_face face, int idx) {
		if (idx > -1 && idx < NUM_VERTS) {
			vert = face.verts[idx];
			u = face.u[idx];
			v = face.v[idx];
		}
	}

	void store_to_face (pcs_insig_face& face, int idx) {
		if (idx > -1 && idx < NUM_VERTS) {
			face.verts[idx] = vert;
			face.u[idx] = u;
			face.v[idx] = v;
		}
	}
};

class insignia_vert_ctrl :
	public editor<pcs_insig_face_vert>
{
protected:
	vector_ctrl* vert;
	float_ctrl* u;
	float_ctrl* v;

public:
	
	insignia_vert_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title, int orient = wxVERTICAL)
	:editor<pcs_insig_face_vert>(parent,x,y,w,h, orient, Title)
	{
		add_control(vert =new vector_ctrl(this,0,0,60,40,_("Position")),0,wxEXPAND );
		add_control(u =new float_ctrl(this,0,0,60,40,_("u")),0,wxEXPAND );
		add_control(v =new float_ctrl(this,0,0,60,40,_("v")),0,wxEXPAND );
	};

	virtual ~insignia_vert_ctrl(void){};

	//set the control's value
	void set_value(const pcs_insig_face_vert&t){
		vert->set_value(t.vert);
		u->set_value(t.u);
		v->set_value(t.v);
	}

	//return's the control's value
	pcs_insig_face_vert get_value(){
		pcs_insig_face_vert vertex;
		vertex.vert = vert->get_value();
		vertex.u = u->get_value();
		vertex.v = v->get_value();
		return vertex;
	}

};

class insignia_vert_array_ctrl :
	public type_array_ctrl<pcs_insig_face_vert, insignia_vert_ctrl>
{
	public:
		insignia_vert_array_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title, wxString subTitle)
			:type_array_ctrl<pcs_insig_face_vert, insignia_vert_ctrl>(parent,x,y,w,h,Title, subTitle, wxVERTICAL, wxEXPAND, ARRAY_LIST, false)
		{
		}
};

class insignia_face_ctrl :
	public editor<pcs_insig_face>
{
protected:
	insignia_vert_array_ctrl* verts;

public:
	
	insignia_face_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title, int orient = wxVERTICAL)
	:editor<pcs_insig_face>(parent,x,y,w,h, orient, Title)
	{
		add_control(verts =new insignia_vert_array_ctrl(this,0,0,60,410,_("Vertexes"), _("")),0,wxEXPAND );
	};

	virtual ~insignia_face_ctrl(void){};

	//set the control's value
	void set_value(const pcs_insig_face&t){
		std::vector<pcs_insig_face_vert> vertexes;
		vertexes.reserve(3);
		for (int i = 0; i < NUM_VERTS; i++) {
			vertexes.push_back(pcs_insig_face_vert(t, i));
		}
		verts->set_value(vertexes);
	}

	//return's the control's value
	pcs_insig_face get_value(){
		pcs_insig_face face;
		const std::vector<pcs_insig_face_vert>& vertexes = verts->get_value();
		for (int i = 0; i < NUM_VERTS; i++) {
			face.verts[i] = vertexes[i].vert;
			face.u[i] = vertexes[i].u;
			face.v[i] = vertexes[i].v;
		}
		return face;
	}

};

class insignia_face_array_ctrl
	:public type_array_ctrl<pcs_insig_face, insignia_face_ctrl>
{
public:
	insignia_face_array_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title, wxString subTitle)
		:type_array_ctrl<pcs_insig_face, insignia_face_ctrl>(parent,x,y,w,h,Title, subTitle, wxVERTICAL, wxEXPAND, ARRAY_LIST)
	{
	}
};

class insignia_ctrl :
	public editor<pcs_insig>
{
protected:
	insignia_face_array_ctrl* faces;
	vector_ctrl*offset;
	int_ctrl*lod;

public:
	
	insignia_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title, int orient = wxVERTICAL)
	:editor<pcs_insig>(parent,x,y,w,h, orient, Title)
	{
		add_control(lod =new int_ctrl(this,0,0,60,40,_("LOD")),0,wxEXPAND );
		add_control(offset =new vector_ctrl(this,0,0,60,40,_("Offset")),0,wxEXPAND );
		add_control(faces=new insignia_face_array_ctrl(this,0,0,60,460,_("Faces"), _("")),0,wxEXPAND );
	};

	virtual ~insignia_ctrl(void){};

	//set the control's value
	void set_value(const pcs_insig&t){
		faces->set_value(t.faces);
		offset->set_value(t.offset);
		lod->set_value(t.lod);
	}

	//return's the control's value
	pcs_insig get_value(){
		pcs_insig insignia;
		insignia.faces = faces->get_value();
		insignia.offset = offset->get_value();
		insignia.lod = lod->get_value();
		return insignia;
	}

	void set_indexes(std::vector<int> i){
		faces->set_indexes(i);
	};

	int get_index() {
		return faces->get_index();
	}
};

class insignia_array_ctrl
	:public type_array_ctrl<pcs_insig, insignia_ctrl>
{
public:
	insignia_array_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title, wxString subTitle)
		:type_array_ctrl<pcs_insig, insignia_ctrl>(parent,x,y,w,h,Title, subTitle, wxVERTICAL, wxEXPAND, ARRAY_LIST)
	{
	}
};


class INSG_ctrl
	:public editor_ctrl<std::vector<pcs_insig> >
{
	insignia_array_ctrl*insignia;

public:
	static color selected_item;
	static color selected_list;
	static color unselected;

	INSG_ctrl(wxWindow*parent)
		:editor_ctrl<std::vector<pcs_insig> >(parent, _("Insignia"))
	{
		//add controls
		add_control(insignia=new insignia_array_ctrl(this,0,0,60,460,_("Insignum"), _("")),0,wxEXPAND );
	}

	//do nothing, needed so the base destructor will get called
	virtual~INSG_ctrl(){}

	//set's the control's value
	void set_value(const std::vector<pcs_insig>&t){
		insignia->set_value(t);
	}

	//return's the control's value
	std::vector<pcs_insig> get_value(){
		return insignia->get_value();
	}

	//populates the control with data from the model
	void set_data(PCS_Model &model){
		set_value(model.get_insignia());
	}

	//applies the data in the control to the model
	void apply_data(PCS_Model &model){
		model.set_insignia(get_value());
	}

	void set_item(const std::vector<int>&coord){
		insignia->set_indexes(coord);
	}
	std::vector<int> get_item(){
		std::vector<int> ret;
		ret.resize(2);
		ret[0] = insignia->get_index();
		ret[1] = insignia->get_child_control()->get_index();
		return ret;
	};

	omnipoints get_omnipoints(){
		omnipoints o;
		//std::vector<pcs_insig> insignia = get_value();
		//for(unsigned int i = 0; i<insignia.size(); i++){
		if (insignia->get_index() != -1) {
			pcs_insig insig = insignia->get_curent_value();
			o.point.reserve(insig.faces.size());
			for (unsigned int j = 0; j < insig.faces.size(); j++) {
				pcs_insig_face face;
				std::vector<omnipoint> points;
				points.resize(NUM_VERTS);
				for (unsigned int k = 0; k < NUM_VERTS; k++) {
					points[k].pos = insig.faces[j].verts[k] + insig.offset;
					points[k].model = -1;
				}
				o.point.push_back(points);
			}
			//}
			o.flags = OMNIPOINT_CLOSED_PATH;
			o.lod = insig.lod;
			o.selected_item = selected_item;
			o.selected_list = selected_list;
			o.unselected = unselected;
	}
		return o;
	}

	void set_omnipoints(const omnipoints&points_){
		// TODO
	}

	void get_omnipoint_coords(int&list, int&item){
		/*list = 0;
		for (int i = 0; i < insignia->get_index(); i++) {
			list += data[i].faces.size();
		}*/
		list = insignia->get_child_control()->get_index();
		item = -1;//points->get_index();
	}
	void set_omnipoint_coords(int&list, int&item){
		// TODO
	}
};
#endif
