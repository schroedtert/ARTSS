/// \file       Utility.cpp
/// \brief      Tests for src/utility/Utility.cpp
/// \date       October 10, 2020
/// \author     N16h7 Hut3r
/// \copyright  <2015-2020> Forschungszentrum Juelich GmbH. All rights reserved.

#include <iostream>
#include <vector>

#include "gtest/gtest.h"

#include "src/Domain.h"
#include "src/Field.h"
#include "src/boundary/BoundaryController.h"
#include "src/advection/ExplicitAdvect.h"


static char xml_buffer[] =
"<JuROr>"
    "<physical_parameters>"
        "<dt>.25</dt>"
    "</physical_parameters>"
    "<domain_parameters>"
        "<X1>0.</X1>"
        "<X2>1.</X2>"
        "<Y1>0.</Y1>"
        "<Y2>1.</Y2>"
        "<Z1>0.</Z1>"
        "<Z2>1.</Z2>"
        "<x1>0.</x1>"
        "<x2>1.</x2>"
        "<y1>0.</y1>"
        "<y2>1.</y2>"
        "<z1>0.</z1>"
        "<z2>1.</z2>"
        "<nx>4</nx>"
        "<ny>4</ny>"
        "<nz>4</nz>"
    "</domain_parameters>"

    "<solver description=\"NSTempTurbSolver\">"
        "<pressure>"
            "<n_level>6</n_level>"
        "</pressure>"
    "</solver>"

    "<boundaries>"
        "<boundary field=\"u,v,w\" patch=\"bottom\""
            " type=\"dirichlet\" value=\"0.0\" />"
        "<boundary field=\"u,v,w\" patch=\"front,back,top,left,right\""
            " type=\"neumann\" value=\"0.0\" />"
        "<boundary field=\"p\" patch=\"bottom\""
            " type=\"neumann\" value=\"0.0\" />"
        "<boundary field=\"p\" patch=\"front,back,top,left,right\""
            " type=\"dirichlet\" value=\"0.0\" />"
        "<boundary field=\"T\" patch=\"front,back,top,left,right\""
            " type=\"neumann\" value=\"0.0\" />"
        "<boundary field=\"T\" patch=\"bottom\""
            " type=\"neumann\" value=\"0.0\" />"
    "</boundaries>"
    "<obstacles enabled=\"No\"></obstacles>"
    "<surfaces enabled=\"No\"></surfaces>"

    "<logging file=\"Test_NavierStokesTempTurb_OpenPlume.log\" level=\"error\"/>"
"</JuROr>";


TEST(ExplicitAdvect, emptyInput) {

    auto xml_file = fmemopen(xml_buffer, sizeof(xml_buffer), "rb");

    auto params = Parameters::getInstance();
    params->parse(xml_file);

    auto domain = Domain::getInstance();
    const int nx = static_cast<int>(domain->Getnx(0));
    const int ny = static_cast<int>(domain->Getny(0));
    const int nz = static_cast<int>(domain->Getnz(0));

    Field out(RHO, 0.0);
    Field in(RHO, 0.0);
    Field u_vel(U, 0.0);
    Field v_vel(V, 0.0);
    Field w_vel(W, 0.0);

    ExplicitAdvect exp_adv = ExplicitAdvect();
    exp_adv.advect(&out, &in, &u_vel, &v_vel, &w_vel, true);

    for(int i=0; i < nx*ny*nz; i++)
        EXPECT_EQ(0.0, out.data[i]);
}

TEST(ExplicitAdvect, OneU) {

    auto xml_file = fmemopen(xml_buffer, sizeof(xml_buffer), "rb");

    auto params = Parameters::getInstance();
    params->parse(xml_file);

    auto domain = Domain::getInstance();
    const int nx = static_cast<int>(domain->Getnx(0));
    const int ny = static_cast<int>(domain->Getny(0));
    const int nz = static_cast<int>(domain->Getnz(0));

    Field out(RHO, 0.0);
    Field in(RHO, 0.0);
    Field u_vel(U, 1.0);
    Field v_vel(V, 0.0);
    Field w_vel(W, 0.0);

    // in.data should look like this
    // 0 0 0 0 
    // 0 1 1 0 
    // 0 1 1 0 
    // 0 0 0 0
    for (int i=1; i<5; i++) {
        for (int j=2; j<4; j++) {
            in.data[i*36 + j*6 + 2] = 1.0;
            in.data[i*36 + j*6 + 3] = 1.0;
        }
    }


    ExplicitAdvect exp_adv = ExplicitAdvect();
    exp_adv.advect(&out, &in, &u_vel, &v_vel, &w_vel, true);

    // out.data should look like this
    // 0 0 0 0 
    // 0 0 1 1 
    // 0 0 1 1 
    // 0 0 0 0 
    for (int i=1; i<5; i++) {
        for (int j=2; j<4; j++) {
            // Expecting a schift by one to resp. to in
            EXPECT_NEAR(out.data[i*36 + j*6 + 3], 1.0, 1E-9);
            EXPECT_NEAR(out.data[i*36 + j*6 + 4], 1.0, 1E-9);
        }
    }
}

TEST(ExplicitAdvect, MinusOneU) {

    auto xml_file = fmemopen(xml_buffer, sizeof(xml_buffer), "rb");

    auto params = Parameters::getInstance();
    params->parse(xml_file);

    auto domain = Domain::getInstance();
    const int nx = static_cast<int>(domain->Getnx(0));
    const int ny = static_cast<int>(domain->Getny(0));
    const int nz = static_cast<int>(domain->Getnz(0));

    Field out(RHO, 0.0);
    Field out2(RHO, 0.0);
    Field in(RHO, 0.0);
    Field u_vel(U, -1.0);
    Field v_vel(V, 0.0);
    Field w_vel(W, 0.0);

    for(int i=1; i<5; i++) {
        for(int j=2; j<4; j++) {
            in.data[i*36 + j*6 + 2] = 1.0;
            in.data[i*36 + j*6 + 3] = 1.0;
        }
    }


    ExplicitAdvect exp_adv = ExplicitAdvect();
    exp_adv.advect(&out, &in, &u_vel, &v_vel, &w_vel, true);
    exp_adv.advect(&out2, &out, &u_vel, &v_vel, &w_vel, true);

    for(int i=1; i<nx*ny*nz; i++) {
        if(in.data[i] != out.data[i])
            std::cout << i << ": " << out.data[i] << std::endl;
    }

    for (int i=1; i<5; i++) {
        for (int j=1; j<5; j++)
            std::cout << out2.data[36 + 6*i + j] << " ";
        std::cout << std::endl;
    }
}
