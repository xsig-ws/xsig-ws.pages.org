import java.util.*;
import java.text.*;
import java.io.*;

class GRCH_Test
{
  public static void main(String[] args) {
    if (args.length < 3) {
      System.out.println("GRCH_Test problem_num result file_coordinates");
      System.exit(1);
    }
    final String path_base =
      "/home/yokoyama/program/grichalle/grch-otameshi/data";
    int problem_num = Integer.parseInt(args[0]);
    StringBuffer pnum_str = new StringBuffer();
    NumberFormat fmt = NumberFormat.getInstance();
    fmt.setMinimumIntegerDigits(2);
    fmt.setMaximumIntegerDigits(2);
    pnum_str.append(fmt.format(problem_num));
    String path = path_base + pnum_str.toString();
    
    GridChallengeApi.Problem prob =
      GridChallengeApi.get_problem(problem_num, path);
    if (prob == null) {
      System.out.println("Error"); System.exit(1);
    }
    System.out.println(prob.key_string);
    //    System.out.println(prob.width);
    //    System.out.println(prob.height);

    String files =
      GridChallengeApi.get_problem_files(problem_num, prob.key_string, path);
    //      GridChallengeApi.get_problem_files(1, "hoge-fugafuga", path);

    try {
      Thread.sleep(3000);
    }
    catch (Exception e) {
    }

    int result =
      GridChallengeApi.answer_problem(problem_num, prob.key_string,
				      Integer.parseInt(args[1]),
				      args[2],
				      path);
  }
}
