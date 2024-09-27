int
dummy(
    int a
    )
{
  int b = a + 1;
  return b;
}

int
main(
    int argc,
    char *argv[]
    )
{
  int a = 2;

  a = dummy(a);

  return 0;
}
