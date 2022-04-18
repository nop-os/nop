int main(int argc, const char **argv, const char **envp);

int _start(const char **argv, const char **envp) {
  int argc = 0;
  while (argv[argc]) argc++;
  
  return main(argc, argv, envp);
}
