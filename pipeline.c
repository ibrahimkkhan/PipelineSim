
/*
 * 
 * pipeline.c
 * 
 */


#include <stdlib.h>
#include "fu.h"
#include "string.h"
#include "pipeline.h"


void
writeback(state_t *state, int *num_insn) {
    const op_info_t *op_info, *op_info_fp;
    int imm, imm_fp;
    int instr = state->int_wb.instr;
    int instr_fp = state->fp_wb.instr;

    if ((state->halt_activated == 1) && 
        (fu_int_done(state->fu_int_list)) && 
        (fu_fp_done(state->fu_add_list)) && 
        (fu_fp_done(state->fu_mult_list)) && 
        (fu_fp_done(state->fu_div_list))&&
        (state->int_wb.instr==0) &&
        (state->fp_wb.instr==0)){
            state->halt = 1;
    }

    

    if(instr != 0){ // Checking whether a instruction is waiting to be written
        op_info = decode_instr(instr, &imm);
        op_info_fp = decode_instr(instr, &imm_fp);
        *num_insn = *num_insn + 1;
        
        switch (op_info->fu_group_num)
        {
        case FU_GROUP_MEM:
            switch (op_info->operation)
            {
                
                unsigned int target_reg, imm_reg, r1_reg, r1_value, r1_value_fp;
                target_reg = FIELD_R2(instr);
                imm_reg = FIELD_IMM(instr);
                r1_reg = FIELD_R1(instr);
                r1_value = state->rf_int.reg_int[r1_reg].w;
                r1_value_fp = state->rf_fp.reg_fp[r1_reg];

                case OPERATION_LOAD: // lw rd r1 imm has rd in FIELD_R2
                    switch (op_info->data_type) //writinh back instructions
                    {
                    case DATA_TYPE_W:
                        memcpy(&(state->rf_int.reg_int[target_reg].w), &(state->mem[r1_value + imm_reg]), 4);
                    break;
                    
                    case DATA_TYPE_F:
                        memcpy(&(state->rf_fp.reg_fp[target_reg]), &(state->mem[r1_value_fp + imm_reg]), 4);
                    break;
                    }
              
                    
                break;
            
                case OPERATION_STORE:
                    switch (op_info->data_type)
                    {
                        case DATA_TYPE_W:
                            memcpy(&(state->mem[r1_value + imm_reg]), &(state->rf_int.reg_int[target_reg].w), 4);
                        break;
                        
                        case DATA_TYPE_F:
                            memcpy(&(state->mem[r1_value_fp + imm_reg]), &(state->rf_fp.reg_fp[target_reg]), 4);
                        break;
                    }
                    
                    break;
                default:
                    break;
            }
            break;

        case FU_GROUP_INT:
            switch (op_info->operation)
            {
            unsigned int operand1_add, operand2_add, result_add; //address
            operand_t operand1, operand2, result; 
            case OPERATION_ADDU:
                if(imm == 1){
                    operand1_add = FIELD_R1(instr);
                    result_add = FIELD_R2(instr); 
                    operand1.integer.wu = (state->rf_int).reg_int[operand1_add].wu;
                    operand2.integer.wu = FIELD_IMM(instr);
                }else{
                    operand1_add = FIELD_R1(instr);  //rs1 address
                    operand2_add = FIELD_R2(instr);  //rs2 address 
                    result_add = FIELD_R3(instr);    //rd  address
                    operand1.integer.wu = (state->rf_int).reg_int[operand1_add].wu;   //rs1 value
                    operand2.integer.wu = (state->rf_int).reg_int[operand2_add].wu; 
                }
                result = perform_operation(instr, state->pc, operand1, operand2);
                (state->rf_int).reg_int[result_add].wu = result.integer.wu;
                break;

            case OPERATION_SUBU:
                if(imm == 1){
                    operand1_add = FIELD_R1(instr);
                    result_add = FIELD_R2(instr); 
                    operand1.integer.wu = (state->rf_int).reg_int[operand1_add].wu;
                    operand2.integer.wu = FIELD_IMM(instr);
                }else{
                    operand1_add = FIELD_R1(instr);  //rs1 address
                    operand2_add = FIELD_R2(instr);  //rs2 address 
                    result_add = FIELD_R3(instr);    //rd  address
                    operand1.integer.wu = (state->rf_int).reg_int[operand1_add].wu;   //rs1 value
                    operand2.integer.wu = (state->rf_int).reg_int[operand2_add].wu; 
                }
                result = perform_operation(instr, state->pc, operand1, operand2);
                (state->rf_int).reg_int[result_add].wu = result.integer.wu;
                break;
            
            case OPERATION_SGTU:
                if(imm == 1){
                    operand1_add = FIELD_R1(instr);
                    result_add = FIELD_R2(instr); 
                    operand1.integer.wu = (state->rf_int).reg_int[operand1_add].wu;
                    operand2.integer.wu = FIELD_IMM(instr);
                }else{
                    operand1_add = FIELD_R1(instr);  //rs1 address
                    operand2_add = FIELD_R2(instr);  //rs2 address 
                    result_add = FIELD_R3(instr);    //rd  address
                    operand1.integer.wu = (state->rf_int).reg_int[operand1_add].wu;   //rs1 value
                    operand2.integer.wu = (state->rf_int).reg_int[operand2_add].wu; 
                }
                result = perform_operation(instr, state->pc, operand1, operand2);
                (state->rf_int).reg_int[result_add].wu = result.integer.wu;
                break;


            case OPERATION_SLTU:
                if(imm == 1){
                    operand1_add = FIELD_R1(instr);
                    result_add = FIELD_R2(instr); 
                    operand1.integer.wu = (state->rf_int).reg_int[operand1_add].wu;
                    operand2.integer.wu = FIELD_IMM(instr);
                }else{
                    operand1_add = FIELD_R1(instr);  //rs1 address
                    operand2_add = FIELD_R2(instr);  //rs2 address 
                    result_add = FIELD_R3(instr);    //rd  address
                    operand1.integer.wu = (state->rf_int).reg_int[operand1_add].wu;   //rs1 value
                    operand2.integer.wu = (state->rf_int).reg_int[operand2_add].wu; 
                }
                result = perform_operation(instr, state->pc, operand1, operand2);
                (state->rf_int).reg_int[result_add].wu = result.integer.wu;
                break;

            
            default:
                 //value

                if(imm == 1){
                    operand1_add = FIELD_R1(instr);
                    result_add = FIELD_R2(instr); 
                    operand1.integer.w = (state->rf_int).reg_int[operand1_add].w;
                    operand2.integer.w = FIELD_IMM(instr);
                }else{
                    operand1_add = FIELD_R1(instr);  //rs1 address
                    operand2_add = FIELD_R2(instr);  //rs2 address 
                    result_add = FIELD_R3(instr);    //rd  address
                    operand1.integer.w = (state->rf_int).reg_int[operand1_add].w;   //rs1 value
                    operand2.integer.w = (state->rf_int).reg_int[operand2_add].w; 
                }
                result = perform_operation(instr, state->pc, operand1, operand2);
                (state->rf_int).reg_int[result_add].w = result.integer.w;
                break;
            }
            break;
        case FU_GROUP_BRANCH:
            state->pc = state->pc_temp_b + 4;
            state->if_id.pc = state->pc_temp_b;
            memcpy( &((state->if_id).instr), &(state->mem[state->if_id.pc]), 4);
            break;
        case FU_GROUP_HALT:
            break;

        // case FU_GROUP_NONE:
        //     *num_insn = *num_insn - 1;
        //     break;
            
        }

        switch (op_info_fp->fu_group_num)
        {
            unsigned int operand1_add, operand2_add, result_add; //address
            operand_t operand1, operand2, result; 

            case FU_GROUP_ADD:
            *num_insn = *num_insn+1;	//increase number of instruction 
            operand1_add = FIELD_R1(instr_fp);
            operand2_add = FIELD_R2(instr_fp);
            result_add = FIELD_R3(instr_fp);
            operand1.flt = (state->rf_fp).reg_fp[operand1_add];
            operand2.flt = (state->rf_fp).reg_fp[operand2_add];
            result = perform_operation(instr_fp, state->pc, operand1, operand2);
            (state->rf_fp).reg_fp[result_add] = result.flt;
                break;
            
            case FU_GROUP_MULT:
                *num_insn = *num_insn+1;	//increase number of instruction 
                operand1_add = FIELD_R1(instr_fp);
                operand2_add = FIELD_R2(instr_fp);
                result_add = FIELD_R3(instr_fp);
                operand1.flt = (state->rf_fp).reg_fp[operand1_add];
                operand2.flt = (state->rf_fp).reg_fp[operand2_add];
                result = perform_operation(instr_fp, state->pc, operand1, operand2);
                (state->rf_fp).reg_fp[result_add] = result.flt;
                break;
            
            case FU_GROUP_DIV:
                *num_insn = *num_insn+1;	//increase number of instruction 
                operand1_add = FIELD_R1(instr_fp);
                operand2_add = FIELD_R2(instr_fp);
                result_add = FIELD_R3(instr_fp);
                operand1.flt = (state->rf_fp).reg_fp[operand1_add];
                operand2.flt = (state->rf_fp).reg_fp[operand2_add];
                result = perform_operation(instr_fp, state->pc, operand1, operand2);
                (state->rf_fp).reg_fp[result_add] = result.flt;
                break;

        }
    }
    
    ////printf("stall cycles is %d", state->stall);
    (state->int_wb.instr) = 0;
    (state->fp_wb.instr) = 0;
}


void
execute(state_t *state) {
    fu_int_t *fu_int;

    if (state->fetch_lock){
        if(state->stall != 0){
            state->stall--;
        }else{
            if(!state->halt_activated){
                state->fetch_lock = FALSE;
            }
            
        }
    }

    

    advance_fu_int(state->fu_int_list, &(state->int_wb));
    advance_fu_fp(state->fu_add_list, &(state->fp_wb));
    advance_fu_fp(state->fu_mult_list, &(state->fp_wb));
    advance_fu_fp(state->fu_div_list, &(state->fp_wb));
}

int extract_destination(int instr){
    const op_info_t *op_info;
    int use_imm;

    op_info = decode_instr(instr, &use_imm);

    switch (op_info->fu_group_num)
    {
    case FU_GROUP_MEM:
        return FIELD_R2(instr);
        break;
    case FU_GROUP_INT:
        if(use_imm == 0){return FIELD_R3(instr);}
        if(use_imm == 1){return FIELD_R2(instr);}
        break;
    case FU_GROUP_DIV:
        return FIELD_R3(instr);
        break;
    case FU_GROUP_ADD:
        return FIELD_R3(instr);
        break;
    case FU_GROUP_MULT:
        return FIELD_R3(instr);
        break;
    default:
        break;
    }

}


int hazard_finder_FP_helper(state_t *state, int reg_1, int reg_2, int target){
    fu_fp_stage_t *stage_fu_add, *stage_fu_div, *stage_fu_mult;
    fu_int_stage_t *stage_int;
    int RAW_int = 0;
    int h_reg; 

    stage_int = state->fu_int_list->stage_list;
    stage_fu_add = state->fu_add_list->stage_list ;
    stage_fu_div = state->fu_div_list->stage_list ;
    stage_fu_mult = state->fu_mult_list->stage_list ;

    while (stage_int != NULL){
        RAW_int++;
        if(stage_int->current_cycle != -1){
            h_reg = extract_destination (stage_int->instr);
            if (h_reg == reg_1 || h_reg == reg_2 || h_reg == target){
                state->stall += RAW_int;
                //printf("h_reg is %d\n", h_reg);
                //printf("Stall in FU cycles is %d", state->stall);
                return 1;
            }

        }
        stage_int = stage_int->prev;
    }

    while (stage_fu_add != NULL){
        RAW_int++;
        if(stage_fu_add->current_cycle != -1){
            h_reg = extract_destination (stage_fu_add->instr);
            if (h_reg == reg_1 || h_reg == reg_2 || h_reg == target){
                state->stall += RAW_int;
                //////printf("Stall in FU cycles is %d", state->stall);
                return 1;
            }

        }
        stage_fu_add = stage_fu_add->prev;
    }

    while (stage_fu_div != NULL){
        RAW_int++;
        if(stage_fu_div->current_cycle != -1){
            h_reg = extract_destination (stage_fu_div->instr);
            if (h_reg == reg_1 || h_reg == reg_2 || h_reg == target){
                state->stall += RAW_int;
                //////printf("Stall in FU cycles is %d", state->stall);
                return 1;
            }

        }
        stage_fu_div = stage_fu_div->prev;
    }

    while (stage_fu_mult != NULL){
        RAW_int++;
        if(stage_fu_mult->current_cycle != -1){
            h_reg = extract_destination (stage_fu_mult->instr);
            if (h_reg == reg_1 || h_reg == reg_2 || h_reg == target){
                state->stall += RAW_int;
                //////printf("Stall in FU cycles is %d", state->stall);
                return 1;
            }

        }
        stage_fu_mult = stage_fu_mult->prev;
    }
    return 0;
}


int hazard_finder(state_t *state, int reg_1, int reg_2, int target){
    const op_info_t *op_info; 
	int imm_d;
    fu_int_stage_t *stage_int;
    fu_fp_stage_t *stage_fu;
    int RAW_int;
    int h_reg; 
    int helper;
    
	op_info = decode_instr((state->if_id).instr, &imm_d); 

    switch (op_info->fu_group_num)
    {
    case FU_GROUP_MEM:
        switch (op_info->data_type)
        {
        case DATA_TYPE_W:
            stage_int = state->fu_int_list->stage_list ;
            RAW_int = 0;
            //checking for instructions in wb
            h_reg = extract_destination(state->int_wb.instr); 
            //passing the instruction in wb to check if the instruction in wb uses the same registers as the current insruction or not
            // after receiving the current register that is being written to, checking if that register matches any of the regsiters we are using
            //in the current instruction

            if (h_reg == reg_1 || h_reg == reg_2 || h_reg == target){
                state->stall++; //RAW
                //////printf("RAW or WAW stall cycles is %d", state->stall); //both RAW and WAW taken care of here
                return 1;
            }
            // if (imm_d == 0){ // not using immediate
            //     else if (h_reg == target){
            //         state->stall++; //WAW
            //         //printf("WAW stall cycles is %d", state->stall);
            //         return 1;
            //     }
            // }else{
            //      if (h_reg == reg_1){
            //         state->stall++; //RAW
            //         //printf("RAW stall cycles is %d", state->stall);
            //         return 1;
            //      }
            // }

        
            while (stage_int != NULL){
                RAW_int++;
                if(stage_int->current_cycle != -1){
                    h_reg = extract_destination (stage_int->instr);
                    if (h_reg == reg_1 || h_reg == reg_2 || h_reg == target){
                        state->stall += RAW_int;
                        //////printf("Stall in FU cycles is %d", state->stall);
                        return 1;
                    }

                }
                stage_int = stage_int->prev;
            }
            break;
        case DATA_TYPE_F:
            stage_int = state->fu_int_list->stage_list ;
            RAW_int = 0;
            //checking for instructions in wb
            h_reg = extract_destination(state->fp_wb.instr); 
            //passing the instruction in wb to check if the instruction in wb uses the same registers as the current insruction or not
            // after receiving the current register that is being written to, checking if that register matches any of the regsiters we are using
            //in the current instruction

            if (h_reg == reg_1 || h_reg == reg_2 || h_reg == target){
                state->stall++; //RAW
                ////printf("h_reg is %d\n", h_reg);
                ////printf("RAW or WAW stall cycles is %d and he_reg is %d\n", state->stall, h_reg); //both RAW and WAW taken care of here
                return 1;
            }
        
            
            helper = hazard_finder_FP_helper(state, reg_1, reg_2, target);
            if (helper == 1){
                return 1;
            }
            break;

        default:
            break;
        }

    break;
    case FU_GROUP_INT:
        stage_int = state->fu_int_list->stage_list ;
        RAW_int = 0;
        //checking for instructions in wb
        h_reg = extract_destination(state->int_wb.instr); 
        //passing the instruction in wb to check if the instruction in wb uses the same registers as the current insruction or not
        // after receiving the current register that is being written to, checking if that register matches any of the regsiters we are using
        //in the current instruction

        if (h_reg == reg_1 || h_reg == reg_2 || h_reg == target){
            state->stall++; //RAW
            //////printf("RAW or WAW stall cycles is %d", state->stall); //both RAW and WAW taken care of here
            return 1;
        }
        // if (imm_d == 0){ // not using immediate
        //     else if (h_reg == target){
        //         state->stall++; //WAW
        //         //printf("WAW stall cycles is %d", state->stall);
        //         return 1;
        //     }
        // }else{
        //      if (h_reg == reg_1){
        //         state->stall++; //RAW
        //         //printf("RAW stall cycles is %d", state->stall);
        //         return 1;
        //      }
        // }

    
        while (stage_int != NULL){
            RAW_int++;
            if(stage_int->current_cycle != -1){
                h_reg = extract_destination (stage_int->instr);
                if (h_reg == reg_1 || h_reg == reg_2 || h_reg == target){
                    state->stall += RAW_int;
                    //////printf("Stall in FU cycles is %d", state->stall);
                    return 1;
                }

            }
            stage_int = stage_int->prev;
        }
    break;
    case FU_GROUP_ADD:
        RAW_int = 0;
        //checking for instructions in wb
        h_reg = extract_destination(state->fp_wb.instr); 
        //passing the instruction in wb to check if the instruction in wb uses the same registers as the current insruction or not
        // after receiving the current register that is being written to, checking if that register matches any of the regsiters we are using
        //in the current instruction

        if (h_reg == reg_1 || h_reg == reg_2 || h_reg == target){
            state->stall++; //RAW
            //////printf("RAW or WAW stall cycles is %d", state->stall); //both RAW and WAW taken care of here
            return 1;
        }
        helper = hazard_finder_FP_helper(state, reg_1, reg_2, target);
        if (helper == 1){
            return 1;
        }
        break;

    case FU_GROUP_DIV:
        RAW_int = 0;
        //checking for instructions in wb
        h_reg = extract_destination(state->fp_wb.instr); 
        //passing the instruction in wb to check if the instruction in wb uses the same registers as the current insruction or not
        // after receiving the current register that is being written to, checking if that register matches any of the regsiters we are using
        //in the current instruction

        if (h_reg == reg_1 || h_reg == reg_2 || h_reg == target){
            state->stall++; //RAW
            ////printf("RAW or WAW stall cycles is %d", state->stall); //both RAW and WAW taken care of here
            return 1;
        }
        helper = hazard_finder_FP_helper(state, reg_1, reg_2, target);
        if (helper == 1){
            return 1;
        }
        break;

    case FU_GROUP_MULT:
        RAW_int = 0;
        //checking for instructions in wb
        h_reg = extract_destination(state->fp_wb.instr); 
        //passing the instruction in wb to check if the instruction in wb uses the same registers as the current insruction or not
        // after receiving the current register that is being written to, checking if that register matches any of the regsiters we are using
        //in the current instruction

        if (h_reg == reg_1 || h_reg == reg_2 || h_reg == target){
            state->stall++; //RAW
            //////printf("RAW or WAW stall cycles is %d", state->stall); //both RAW and WAW taken care of here
            return 1;
        }
        helper = hazard_finder_FP_helper(state, reg_1, reg_2, target);
        if (helper == 1){
            return 1;
        }
        break;
    
    default:
        break;
    }

   

    return 0; //Hazards
    //! CODE FOR imm_d == 1 and WAW in imm_d == 0  and imm_d == 1(target == wb)
}



int
decode(state_t *state) {
	const op_info_t *op_info; 
	int imm_d;
    int RAW_check;
    unsigned pc_temp = state->if_id.pc;

	

    if(state->stall == 0 && state->halt_activated != TRUE){
        int instr = (state->if_id).instr;
        // int instr_fp = (state->if_id).
        op_info = decode_instr(instr, &imm_d); 

        switch(op_info->fu_group_num) {
            case FU_GROUP_MEM:
                RAW_check = hazard_finder(state, -1, -1, FIELD_R2(instr));
                if(RAW_check == 1){
                    //printf("hazard found");
                    state->fetch_lock = TRUE;
                }else{
                    if (issue_fu_int(state->fu_int_list, instr) == -1){
                        //printf("hazard found");
                        state->fetch_lock = TRUE;
                        state->stall++;
                    }else{
                        //printf("hazard not found");
                        state->fetch_lock = FALSE;
                    }
                }
            break;

            case FU_GROUP_INT:
                if(imm_d == 1){
                    RAW_check = hazard_finder(state, FIELD_R1(instr), -1, FIELD_R2(instr));
                }else{
                    RAW_check = hazard_finder(state, FIELD_R1(instr), FIELD_R2(instr), FIELD_R3(instr));
                }

                if(RAW_check == 1){
                    //printf("hazard found");
                    state->fetch_lock = TRUE;
                }else{
                    if (issue_fu_int(state->fu_int_list, instr) == -1){
                        //printf("hazard found");
                        state->fetch_lock = TRUE;
                        state->stall++;
                    }else{
                        //printf("hazard not found");
                        state->fetch_lock = FALSE;
                    }
                }
                
            break;

            case FU_GROUP_ADD:
                RAW_check = hazard_finder(state, FIELD_R1(instr), FIELD_R2(instr), FIELD_R3(instr));
                if(RAW_check == 1){
                    //printf("hazard found");
                    state->fetch_lock = TRUE;
                }else{
                    if (issue_fu_fp(state->fu_add_list, instr) == -1){
                        //printf("hazard found");
                        state->fetch_lock = TRUE;
                        state->stall++;
                    }else{
                        //printf("hazard not found");
                        state->fetch_lock = FALSE;
                    }
                }
                break;

            case FU_GROUP_DIV:
                RAW_check = hazard_finder(state, FIELD_R1(instr), FIELD_R2(instr), FIELD_R3(instr));
                if(RAW_check == 1){
                    //printf("hazard found");
                    state->fetch_lock = TRUE;
                }else{
                    if (issue_fu_fp(state->fu_div_list, instr) == -1){
                        //printf("hazard found");
                        state->fetch_lock = TRUE;
                        state->stall++;
                    }else{
                        //printf("hazard not found");
                        state->fetch_lock = FALSE;
                    }
                    
                }
                break;

            case FU_GROUP_MULT:
                RAW_check = hazard_finder(state, FIELD_R1(instr), FIELD_R2(instr), FIELD_R3(instr));
                if(RAW_check == 1){
                    //printf("hazard found");
                    state->fetch_lock = TRUE;
                }else{
                    if (issue_fu_fp(state->fu_mult_list, instr) == -1){
                        //printf("hazard found");
                        state->fetch_lock = TRUE;
                        state->stall++;
                    }else{
                        //printf("hazard not found");
                        state->fetch_lock = FALSE;
                    }
                }
                break;

            case FU_GROUP_BRANCH:
                switch(op_info->operation){	
                    case OPERATION_J:
                        pc_temp = pc_temp + FIELD_OFFSET(instr) +4;	
                        state->stall = 3;
                        break;
                    case OPERATION_JAL:
                        (state->rf_int).reg_int[31].wu = pc_temp;		
                        pc_temp = pc_temp + FIELD_OFFSET(instr) +4;  	
                        state->stall = 3;
                        break;
                    case OPERATION_JR:
                        pc_temp = (state->rf_int).reg_int[ FIELD_R1(instr) ].w ;	
                        state->stall = 3;
                        break;
                    case OPERATION_JALR:
                        (state->rf_int).reg_int[31].wu = pc_temp;				
                        pc_temp = (state->rf_int).reg_int[ FIELD_R1(instr) ].w ;   
                        state->stall = 3;
                        break;
                    case OPERATION_BEQZ:
                        if( (state->rf_int).reg_int[ FIELD_R1(instr) ].w == 0  ){
                            state->stall = 3;
                            pc_temp = pc_temp + FIELD_IMM(instr) + 4;
                        }
                        else{
                            state->stall = 2;
                            pc_temp =  pc_temp + 4;
                        }
                        break;
                    case OPERATION_BNEZ:
                        if( (state->rf_int).reg_int[ FIELD_R1(instr) ].w != 0 ){
                            pc_temp =  pc_temp + FIELD_IMM(instr) + 4 ;
                            state->stall = 3;
                        }
                        else{
                            pc_temp =  pc_temp + 4 ;
                            state->stall = 2;
                        }
                        break;

                
                // if(RAW_check == 1){
                //     //printf("hazard found");
                //     state->fetch_lock = TRUE;
                // }else{
                    
                // }
                }
            state->pc_temp_b = pc_temp;
            state->fetch_lock = TRUE;

		    if (issue_fu_int(state->fu_int_list, instr) == -1)	{
                state->fetch_lock = TRUE;	
            }
			

            break;
            case FU_GROUP_HALT:
                state->fetch_lock = TRUE;
			    state->halt_activated = TRUE;
                
                return 0;
                break;

        }
       

    } 
}


void
fetch(state_t *state) {
    if (state->fetch_lock != TRUE){
        state->if_id.pc = state->pc;
	    memcpy( &((state->if_id).instr), &(state->mem[state->pc]), 4);
	    state->pc += 4;
    }
    
}
