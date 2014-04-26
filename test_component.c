#include <stdbool.h>
#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CuTest.h"

#include "component.h"

typedef struct {
    int type;
    int flag;
    int len;
    char *data;
} msg_t;


void TestComponent_set_size_with_init(CuTest * tc)
{
    cbuf_t *cb;

    cb = cbuf_new(16);
    CuAssertTrue(tc, 1UL << 16 == cbuf_get_size(cb));
}

void TestComponent_set_size_with_init(CuTest * tc)
{
    void* cm;
    void* comp;

    comp = component_new();
//    component_add_mailbox(comp,
    //comp_offer_msg(comp,msg);
    cm_offer_msg(comp,msg("SET_VALUE"),0,"A=1",3);
    if ((msg = comp_poll_msg(comp,mt("SET_VALUE"))))
    {

    }

    ent_send_msg(slf,slf,et("CREATE_ENT"),0,"ATTACK");

    /* ROOM_OCCUPANT */
    void __MSG_CREATE_ENT_DONE(entity_t* slf, msg_t* msg)
    {
        int ent;

        ent = msg_token_int(msg,"createdent");
        ent_send_msg(slf,in(slf)->room,et("PLACE_WITHIN"),0,ent,sizeof(int));
    }

    /* ENTITY */
    void __MSG_CREATE_ENT(entity_t* slf, msg_t* msg)
    {
        entity_t* ent;
        ent = ent_new(msg_token(msg,"etype"),slf);
        ent_send_msg(slf,ent,et("CREATE_ENT_DONE"),0,"",0);
    }

    /* BURNING */
    void __MSG_CREATE_ENT_DONE(entity_t* slf, msg_t* msg)
    {
        ent_send_msg(slf,slf,et("ADD_COMPONENT"),0,component("BURNING"),sizeof(int));
    }

    /* ATTACK */
    void __MSG_STEP(entity_t* slf, msg_t* msg)
    {
        if (msg_token_int(msg,"stepnum") == 1)
        {
            ent_send_msg(slf,slf,et("GET_ROOM"),0,0,0);
        }
        else if (msg_token_int(msg,"stepnum") == 2)
        {
            ent_send_msg(slf,slf,et("ATTACK"),0,0,0);
        }
    }

    void __MSG_CURRENT_ROOM(entity_t* slf, msg_t* msg)
    {
        ent_send_msg(slf,msg_token_int("entid"),et("GET_CONTENTS"),0,0,0);
    }

    void __MSG_CONTENTS(entity_t* slf, msg_t* msg)
    {
        int ii;

        for (ii=0; ii<msg_len(msg) / 4; ii++)
        {
            int ent;

            ent = msg_get_int(msg,ii);
            ent_send_msg(slf,slf,mt("ATTACKING"),0,ent,4);
        }
    }

    void __MSG_INCREASE(entity_t* slf, msg_t* msg)
    {
        in(slf)->damage_pts += 10;
    }

    /* BURNS */
    void __MSG_ATTACKING(entity_t* slf, msg_t* msg)
    {
        int ent;

        ent = msg_get_int(msg,0);
        ent_send_msg(slf,ent,mt("BURN"),0,ent,4);
    }

    /* ICED */
    void __MSG_BURN(entity_t* slf, msg_t* msg)
    {
        int ent;

        ent = msg_get_int(msg,0);
        ent_send_msg(slf,slf,mt("REMOVE COMPONENT"),0,ct("ICED"),4);
    }

    /* AMPLIFIED */
    void __MSG_ATTACKING(entity_t* slf, msg_t* msg)
    {
        ent_send_msg(slf,msg_sender(msg),mt("INCREASE"),0,2,sizeof(int));
    }

    /* INFLAMMABLE */
     void __MSG_BURN(entity_t* slf, msg_t* msg)
    {
        ent_send_msg(slf,msg_sender(msg),mt("KILL"),0,2,sizeof(int));
    }

    
    
    

    ent_send_msg(atk,mt("ATTACK"),0,attack(10,10),2);

    if ((msg = ent_sendrecv_msg(ent,mt("GET_CONTENTS"),0,"",0)))
    {
        if (msg_type(msg) != mt("CONTENTS")) return;

        int i;;

        for (ii=0;ii<msg_len(msg) / 4; ii++)
        {
            int entid;
            int atk;

            entid = msg_get_int(msg,ii);
            if ((atk = ent_sendrecv_msg(self,et("CREATE_ENT"),0,"ATTACK")))
            {
                ent_send_msg(atk,mt("ATTACK"),0,attack(entid),2);
            }
            ent_send_msg(atk,mt("ATTACK"),0,attack(10,10),2);
        }



    }

    if (msg = (ent_recv_msg(ent,mt("CONTENTS"))))
    {

    }


    if (msg = (ent_recv_msg(ent,mt("GET_CONTENTS"))))
    {
        ent_send_msg(ent,msg("CONTENTS"),0,"",0);
        
    }


    cm = component_manager_new();
    component_manager_add_component(comp);


    CuAssertTrue(tc, 1UL << 16 == cbuf_get_size(cb));

}
