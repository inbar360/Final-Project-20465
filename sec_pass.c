void make_code_binary(st){
  Data_Table *curr_node;
  int i = 0, line = 0;
  while(st[i] != '\0'){
    *curr_node == &data_head
    mark_label_entry(st, line);
    while(curr_node->type != 'c' && curr_node->type != 'C'){
      curr_node = &get_next(*curr_node);
    }
    int res = make_command(st, curr_node, line);
    if(res == 1)
        curr_node->type = 'f';//stands for finished with this line
    NEXT_LINE(st, i);
  }
}

//add function that creates .ext and .ent files
