// hidden_star.c.inc

void bhv_hidden_star_init(void) {
    s16 count = count_objects_with_behavior(bhvHiddenStarTrigger);
    if (count == 0) {
        struct Object *obj = spawn_object_abs_with_rot(o, 0, MODEL_STAR, bhvStar, o->oPosX, o->oPosY, o->oPosZ, 0, 0, 0);
        if (obj != NULL) { obj->oBehParams = o->oBehParams; }
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }

    o->oHiddenStarTriggerCounter = 5 - count;
    
    // We haven't interacted with a player yet.
    // We also don't sync this as not only is it not required
    // but it also is only set for an interaction.
    // Therefore this object must already be loaded for it to be set
    // and if it wasn't. You couldn't of possibly been the one
    // who last interacted to begin with.
    o->oHiddenStarLastInteractedObject = NULL;
    
    if (!network_sync_object_initialized(o)) {
        struct SyncObject *so = network_init_object(o, SYNC_DISTANCE_ONLY_EVENTS);
        if (so) {
            network_init_object_field(o, &o->oAction);
            network_init_object_field(o, &o->oHiddenStarTriggerCounter);
            network_init_object_field(o, &o->oPosX);
            network_init_object_field(o, &o->oPosY);
            network_init_object_field(o, &o->oPosZ);
            network_init_object_field(o, &o->oTimer);
        }
    }
}

void bhv_hidden_star_loop(void) {
    switch (o->oAction) {
        case 0:
            if (o->oHiddenStarTriggerCounter == 5) {
                o->oAction = 1;
            }
            break;

        case 1:
            if (o->oTimer > 2) {
                struct Object *obj = spawn_red_coin_cutscene_star(o->oPosX, o->oPosY, o->oPosZ);
                if (obj != NULL) {
                    if (o->oHiddenStarLastInteractedObject == &gMarioStates[0]) {
                        obj->oStarSpawnExtCutsceneFlags = 1;
                    } else {
                        obj->oStarSpawnExtCutsceneFlags = 0;
                    }
                    spawn_mist_particles();
                }
                o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
            }
            break;
    }
}

/* TODO: this is likely not a checkpoint but a Secret */
void bhv_hidden_star_trigger_loop(void) {
    if ((o->oInteractStatus & INT_STATUS_INTERACTED) || obj_check_if_collided_with_object(o, gMarioObjects[0]) == 1) {
        struct Object *hiddenStar = cur_obj_nearest_object_with_behavior(bhvHiddenStar);
        if (hiddenStar != NULL) {

            s16 count = (count_objects_with_behavior(bhvHiddenStarTrigger) - 1);
            hiddenStar->oHiddenStarTriggerCounter = 5 - count;

            if (hiddenStar->oHiddenStarTriggerCounter != 5) {
                spawn_orange_number(hiddenStar->oHiddenStarTriggerCounter, 0, 0, 0);
            }
            
            // Set the last person who interacted with a secret to the 
            // parent so only they get the star cutscene.
            struct MarioState *player = nearest_mario_state_to_object(o);
            if (player) {
                hiddenStar->oHiddenStarLastInteractedObject = player;
            }

#ifdef VERSION_JP
            play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundSource);
#else
            play_sound(SOUND_MENU_COLLECT_SECRET
                           + (((u8) hiddenStar->oHiddenStarTriggerCounter - 1) << 16),
                       gGlobalSoundSource);
#endif
        }

        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
        if (!(o->oInteractStatus & INT_STATUS_INTERACTED)) {
            network_send_collect_item(o);
        }
    }
}

void bhv_bowser_course_red_coin_star_loop(void) {
    gRedCoinsCollected = o->oHiddenStarTriggerCounter;
    switch (o->oAction) {
        case 0:
            if (o->oHiddenStarTriggerCounter == 8) {
                o->oAction = 1;
            }
            break;

        case 1:
            if (o->oTimer > 2) {
                struct Object *obj = spawn_no_exit_star(o->oPosX, o->oPosY, o->oPosZ);
                if (obj != NULL) {
                    if (o->oHiddenStarLastInteractedObject == &gMarioStates[0]) {
                        obj->oStarSpawnExtCutsceneFlags = 1;
                    } else {
                        obj->oStarSpawnExtCutsceneFlags = 0;
                    }
                    spawn_mist_particles();
                }
                o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
            }
            break;
    }
}
