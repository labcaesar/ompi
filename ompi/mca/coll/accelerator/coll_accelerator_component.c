/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2014      The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2014-2015 NVIDIA Corporation.  All rights reserved.
 * Copyright (c) 2015      Los Alamos National Security, LLC. All rights
 *                         reserved.
 * Copyright (c) 2024      Triad National Security, LLC. All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "ompi_config.h"

#include <string.h>

#include "mpi.h"
#include "ompi/constants.h"
#include "coll_accelerator.h"

/*
 * Public string showing the coll ompi_accelerator component version number
 */
const char *mca_coll_accelerator_component_version_string =
    "Open MPI accelerator collective MCA component version " OMPI_VERSION;

/*
 * Local function
 */
static int accelerator_register(void);

/*
 * Instantiate the public struct with all of our public information
 * and pointers to our public functions in it
 */

mca_coll_accelerator_component_t mca_coll_accelerator_component = {
    {
        /* First, the mca_component_t struct containing meta information
         * about the component itself */

        .collm_version = {
            MCA_COLL_BASE_VERSION_3_0_0,

            /* Component name and version */
            .mca_component_name = "accelerator",
            MCA_BASE_MAKE_VERSION(component, OMPI_MAJOR_VERSION, OMPI_MINOR_VERSION,
                                  OMPI_RELEASE_VERSION),

            /* Component open and close functions */
            .mca_register_component_params = accelerator_register,
        },
        .collm_data = {
            /* The component is checkpoint ready */
            MCA_BASE_METADATA_PARAM_CHECKPOINT
        },

        /* Initialization / querying functions */

        .collm_init_query = mca_coll_accelerator_init_query,
        .collm_comm_query = mca_coll_accelerator_comm_query,
    },

    /* accelerator-specific component information */

    /* Priority: make it above all point to point collectives including self */
    .priority = 78,
};


static int accelerator_register(void)
{
    (void) mca_base_component_var_register(&mca_coll_accelerator_component.super.collm_version,
                                           "priority", "Priority of the accelerator coll component; only relevant if barrier_before or barrier_after is > 0",
                                           MCA_BASE_VAR_TYPE_INT, NULL, 0, 0,
                                           OPAL_INFO_LVL_6,
                                           MCA_BASE_VAR_SCOPE_READONLY,
                                           &mca_coll_accelerator_component.priority);

    (void) mca_base_component_var_register(&mca_coll_accelerator_component.super.collm_version,
                                           "disable_accelerator_coll", "Automatically handle the accelerator buffers for the MPI collective.",
                                           MCA_BASE_VAR_TYPE_INT, NULL, 0, 0,
                                           OPAL_INFO_LVL_2,
                                           MCA_BASE_VAR_SCOPE_READONLY,
                                           &mca_coll_accelerator_component.disable_accelerator_coll);

    return OMPI_SUCCESS;
}
