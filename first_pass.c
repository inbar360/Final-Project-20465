void make_line_binary(file first word: st){
  int i = 0, line = 0;
  while(st[i] != '\0'){
    int res = add_string_data(st, line);
    if(res == 1){
      res = add_extern_data(st, line);
      if(res == 2)
        res = make_memory_of_command(st, line);
    }
    else if(res == 2){
      res = add_data_data(st, line);
      if(res == 2){
          res = make_memory_of_command_label(st, line);
          if(res == 2)
            res = make_extern_data(st, line)
      }
    }
    NEW_LINE(st, i);
    line++;
}
