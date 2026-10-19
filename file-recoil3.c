// file-recoil3.c - RECOIL plugin for GIMP 3
//
// Copyright (C) 2021-2026 Piotr Fusik
// SPDX-License-Identifier: MIT
//
// This file is part of RECOIL (Retro Computer Image Library),
// see https://recoil.sourceforge.net

#include <string.h>
#include <libgimp/gimp.h>

#include "recoil-stdio.h"
#include "formats.h"

#define LOAD_PROC "file-recoil-load"

static GimpImage *load_image(GFile *file)
{
	char *filename = g_file_get_path(file);
	if (filename == NULL)
		return NULL;
	RECOIL *recoil = RECOILStdio_New();
	bool ok = RECOILStdio_Load(recoil, filename);
	g_free(filename);
	if (!ok) {
		RECOIL_Delete(recoil);
		return NULL;
	}
	gegl_init(NULL, NULL);
	int width = RECOIL_GetWidth(recoil);
	int height = RECOIL_GetHeight(recoil);
	GimpImage *image = gimp_image_new(width, height, GIMP_RGB);
	if (image != NULL) {
		float x_dpi = RECOIL_GetXPixelsPerInch(recoil);
		if (x_dpi != 0)
			gimp_image_set_resolution(image, x_dpi, RECOIL_GetYPixelsPerInch(recoil));
		GimpLayer *layer = gimp_layer_new(image, "Background", width, height, GIMP_RGB_IMAGE, 100, GIMP_LAYER_MODE_NORMAL);
		gimp_image_insert_layer(image, layer, NULL, 0);
		const Babl *format = babl_format_new(babl_model("R'G'B'"), babl_type("u8"),
#if G_BYTE_ORDER == G_LITTLE_ENDIAN
			babl_component("B'"), babl_component("G'"), babl_component("R'"), babl_component("PAD"),
#else
			babl_component("PAD"), babl_component("R'"), babl_component("G'"), babl_component("B'"),
#endif
			NULL);
		GeglBuffer *buffer = gimp_drawable_get_buffer(&layer->parent_instance);
		gegl_buffer_set(buffer, NULL, 0, format, RECOIL_GetPixels(recoil), GEGL_AUTO_ROWSTRIDE);
		g_object_unref(buffer);
	}
	RECOIL_Delete(recoil);
	return image;
}

static GList *query_procedures(GimpPlugIn *plug_in)
{
	return g_list_append(NULL, g_strdup(LOAD_PROC));
}

static GimpValueArray *load(GimpProcedure *procedure, GimpRunMode run_mode, GFile *file,
	GimpMetadata *metadata, GimpMetadataLoadFlags *flags, GimpProcedureConfig *config, gpointer run_data)
{
	GimpImage *image = load_image(file);
	if (image == NULL)
		return gimp_procedure_new_return_values(procedure, GIMP_PDB_EXECUTION_ERROR, NULL);
	GimpValueArray *return_vals = gimp_procedure_new_return_values(procedure, GIMP_PDB_SUCCESS, NULL);
	GIMP_VALUES_SET_IMAGE(return_vals, 1, image);
	return return_vals;
}

static GimpProcedure *create_procedure(GimpPlugIn *plug_in, const gchar *name)
{
	if (strcmp(name, LOAD_PROC) == 0) {
		GimpProcedure *procedure = gimp_load_procedure_new(plug_in, name, GIMP_PDB_PROC_TYPE_PLUGIN, load, NULL, NULL);
		gimp_procedure_set_documentation(procedure,
			"Loads files using Retro Computer Image Library (RECOIL)",
			"Loads files using Retro Computer Image Library (RECOIL)", name);
		gimp_procedure_set_attribution(procedure,
			"Piotr Fusik <fox@scene.pl>",
			"Piotr Fusik <fox@scene.pl>", "2021-2026");
		gimp_file_procedure_set_extensions(GIMP_FILE_PROCEDURE(procedure), GIMP_RECOIL_EXTS);
		return procedure;
	}
	return NULL;
}

typedef struct {
	GimpPlugIn parent_instance;
} Recoil;

typedef struct {
	GimpPlugInClass parent_class;
} RecoilClass;

static void recoil_class_init(RecoilClass *klass)
{
	GimpPlugInClass *plug_in_class = GIMP_PLUG_IN_CLASS(klass);
	plug_in_class->query_procedures = query_procedures;
	plug_in_class->create_procedure = create_procedure;
	plug_in_class->set_i18n = NULL;
}

static void recoil_init(Recoil *recoil)
{
}

#define RECOIL_TYPE (recoil_get_type())

G_DEFINE_TYPE(Recoil, recoil, GIMP_TYPE_PLUG_IN)
GIMP_MAIN(RECOIL_TYPE)
