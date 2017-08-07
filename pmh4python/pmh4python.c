/* PEG Markdown Highlight
 * Copyright 2011-2016 Ali Rantakari -- http://hasseg.org
 * Licensed under the GPL2+ and MIT licenses (see LICENSE for more info).
 * 
 * highlighter.c
 * 
 * Test program that parses the Markdown content from stdin and outputs
 * the positions of found language elements.
 */

#include <Python.h>
#include <stdio.h>
#include <string.h>
#include "pmh_parser.h"


//Actual module method definition - this is the code that will be called by
//pmh_wrapper.parse_markdown(STRING_TO_PARSE)
static PyObject* parse_markdown(PyObject *self, PyObject *args)
{   

    int N_ELEMENTS = 25;

    // allocate space for N_ELEMENTS pointers to strings
    char **PEG_ELEMENT_TYPES = (char**)malloc(N_ELEMENTS*sizeof(char*));
    int j = 0;

    //allocate space for each string
    for(j = 0; j < N_ELEMENTS; j++){
        PEG_ELEMENT_TYPES[j] = (char*)malloc(15*sizeof(char));
    }


    sprintf(PEG_ELEMENT_TYPES[0],"LINK");               //Explicit link
    sprintf(PEG_ELEMENT_TYPES[1],"AUTO_LINK_URL");      //Implicit URL link
    sprintf(PEG_ELEMENT_TYPES[2],"AUTO_LINK_EMAIL");    //Implicit email link
    sprintf(PEG_ELEMENT_TYPES[3],"IMAGE");              //Image definition
    sprintf(PEG_ELEMENT_TYPES[4],"CODE");               //Code (inline)
    sprintf(PEG_ELEMENT_TYPES[5],"HTML");               //HTML
    sprintf(PEG_ELEMENT_TYPES[6],"HTML_ENTITY");        //HTML special entity definition
    sprintf(PEG_ELEMENT_TYPES[7],"EMPH");               //Emphasized text
    sprintf(PEG_ELEMENT_TYPES[8],"STRONG");             //Strong text
    sprintf(PEG_ELEMENT_TYPES[9],"LIST_BULLET");        //Bullet for an unordered list item
    sprintf(PEG_ELEMENT_TYPES[10],"LIST_ENUMERATOR");    //Enumerator for an ordered list item
    sprintf(PEG_ELEMENT_TYPES[11],"COMMENT");            //(HTML) Comment

    // Code assumes that 'H1'-6 are in order.
    sprintf(PEG_ELEMENT_TYPES[12],"H1");                 //Header, level 1
    sprintf(PEG_ELEMENT_TYPES[13],"H2");                 //Header, level 2
    sprintf(PEG_ELEMENT_TYPES[14],"H3");                 //Header, level 3
    sprintf(PEG_ELEMENT_TYPES[15],"H4");                 //Header, level 4
    sprintf(PEG_ELEMENT_TYPES[16],"H5");                 //Header, level 5
    sprintf(PEG_ELEMENT_TYPES[17],"H6");                 //Header, level 6

    sprintf(PEG_ELEMENT_TYPES[18],"BLOCKQUOTE");         //Blockquote
    sprintf(PEG_ELEMENT_TYPES[19],"VERBATIM");           //Verbatim (e.g. block of code)
    sprintf(PEG_ELEMENT_TYPES[20],"HTMLBLOCK");          //Block of HTML
    sprintf(PEG_ELEMENT_TYPES[21],"HRULE");              //Horizontal rule
    sprintf(PEG_ELEMENT_TYPES[22],"REFERENCE");          //Reference
    sprintf(PEG_ELEMENT_TYPES[23],"NOTE");               //Note
    sprintf(PEG_ELEMENT_TYPES[24],"STRIKE");             //Strike-through

    pmh_element **result;
    char *md_source;
    
    PyArg_ParseTuple(args, "s", &md_source);
    pmh_markdown_to_elements(md_source, pmh_EXT_NONE, &result);
    pmh_sort_elements_by_pos(result);
    
    

    PyObject *types = PyDict_New();

    pmh_element *cursor;
    int i;

    for (i = 0; i < pmh_NUM_LANG_TYPES; i++)
    {   

        cursor = result[i];
        if (cursor == NULL)
            continue;
        
        char *element_type = PEG_ELEMENT_TYPES[i];

        PyObject *list = PyList_New(0);

        while (cursor != NULL)
        {   
            
            PyObject *py_item = PyDict_New();

            PyDict_SetItem(py_item, Py_BuildValue("s", "start"), Py_BuildValue("k", cursor->pos));
            PyDict_SetItem(py_item, Py_BuildValue("s", "end"), Py_BuildValue("k", cursor->end));

            PyList_Append(list, py_item);

            Py_DECREF(py_item);

            cursor = cursor->next;
        }

        PyDict_SetItem(types, Py_BuildValue("s", element_type), list);
        Py_DECREF(list);
    }

    pmh_free_elements(result);

    return types;
}

//Method definition object for this extension, these argumens mean:
//ml_name: The name of the method
//ml_meth: Function pointer to the method implementation
//ml_flags: Flags indicating special features of this method, such as
//          accepting arguments, accepting keyword arguments, being a
//          class method, or being a static method of a class.
//ml_doc:  Contents of this method's docstring
static PyMethodDef pmh_module_methods[] = { 
    {   
        "parse_markdown",
        parse_markdown,
        METH_VARARGS,
        "Retreive pmh_elements from a method defined in a C extension."
    },  
    {NULL, NULL, 0, NULL}
};

//Module definition
//The arguments of this structure tell Python what to call your extension,
//what it's methods are and where to look for it's method definitions
static struct PyModuleDef pmh_module_definition = { 
    PyModuleDef_HEAD_INIT,
    "highlighter",
    "A Python wrapper around peg-markdown-highlighter from C code.",
    -1, 
    pmh_module_methods
};

//Module initialization
//Python calls this function when importing your extension. It is important
//that this function is named PyInit_[[your_module_name]] exactly, and matches
//the name keyword argument in setup.py's setup() call.
PyMODINIT_FUNC PyInit_pmh4python(void)
{

    Py_Initialize();

    return PyModule_Create(&pmh_module_definition);;
}

