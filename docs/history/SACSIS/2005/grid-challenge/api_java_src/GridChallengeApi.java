import java.security.*;
import java.io.*;
import java.text.*;
import java.util.*;
import java.nio.*;
import java.nio.channels.*;

public class GridChallengeApi
{
  public static class Problem
  {
    public String key_string;
    public int width;
    public int height;
  }

  private static class Challenge_data
  {
    int prob_num;
    int w;
    int h;
    long start_time;
    static final long base_time;
    static {
      Calendar c = Calendar.getInstance();
      c.set(2004, 11, 1, 0, 0, 0); // 2004/12/01 00:00:00
      base_time = c.getTime().getTime();
    }
  }

  private static class Answer_file
  {
    BufferedReader fstream;
    boolean is_encripted;
    int prob_num;
    int w;
    int h;
    public boolean open(String fname) {
      File fp = new File(fname);
      if (!fp.exists() || !fp.canRead() || fp.isDirectory()) {
	fp = null;
	return false;
      }
      try {
	FileReader fr = new FileReader(fp);
	fstream = new BufferedReader(fr);
      }
      catch (FileNotFoundException e) {
	return false;
      }
      return true;
    }
    public boolean read_header() {
      try {
	String s = fstream.readLine();
	if (s == null) return false;
	StringTokenizer st = new StringTokenizer(s, " ");
	if (st.countTokens() != 4) {
	  return false;
	}
	prob_num = Integer.parseInt(st.nextToken());
	w = Integer.parseInt(st.nextToken());
	h = Integer.parseInt(st.nextToken());
	int e = Integer.parseInt(st.nextToken());
	is_encripted = (e == 0 ? false : true);
	return true;
      }
      catch (IOException e) {
	return false;
      }
    }
    public boolean read_body(part_ans[] ans) {
      //XXX todo: encrypted
      try {
	for (int i = 0; i < w * h; i++) {
	  String s = fstream.readLine();
	  if (s == null) return false;
	  StringTokenizer st = new StringTokenizer(s, " ");
	  if (st.countTokens() != 4) {
	    return false;
	  }
	  String coord_str = st.nextToken();
	  if (prob_num != cut_and_atoi(coord_str, 0, 2)) return false;
	  int x = cut_and_atoi(coord_str, 2, 2 + 3);
	  int y = cut_and_atoi(coord_str, 5, 5 + 3);
	  int inner = Integer.parseInt(st.nextToken());
	  int left = Integer.parseInt(st.nextToken());
	  int upper = Integer.parseInt(st.nextToken());
	  ans[coord_a(x, y, w)].set(inner, left, upper);
	}
	return true;
      }
      catch (IOException e) {
	return false;
      }
    }
    public static int cut_and_atoi(String s, int begin, int end) {
      return Integer.parseInt(s.substring(begin, end));
    }
    
  }
  private static int coord_a(int x, int y, int width) {
    return y * width + x;
  }
  private static class part_ans
  {
    int total;
    int left;
    int upper;
    public void set(int t, int l, int u) {
      total = t; left = l; upper = u;
    }
  }

  private static Challenge_data challenge_data;
  static {
    challenge_data = new Challenge_data();
  }
  /**
   * \fn Problem get_problem(int problem_number, String config_dir)
   * \param problem_number : 問題番号
   * \param config_dir : お試し版で正答ファイルの置いてあるディレクトリを指定する 予選・決勝ではNULLとすること
   * \return null / 異常
   */
  public static Problem
  get_problem(int problem_number, String config_dir_org) {
    try {
      int stage = P_INVALID;
      if ((stage = problem_number_check(problem_number)) == P_INVALID) {
	grch_out("problem_number is wrong"); return null;
      }
      //XXX todo: check otameshi, yosen, kessyou

      String config_dir = get_problem_dir(problem_number, config_dir_org);
      String config_str = answer_fname(config_dir, problem_number);
      if (config_str == null) {
	grch_out("config_dir is wrong"); return null;
      }
      Answer_file ans_f = new Answer_file();
      
      if (!ans_f.open(config_str)) {
	if (stage == P_OTAMESHI) {
	  grch_out("cannot find config file in " + config_dir); return null;
	} else {
	  grch_out("Internal error. cannot find config file"); return null;
	}
      }
      if (!ans_f.read_header()) {
	if (stage == P_OTAMESHI) {
	  grch_out("cannot read config file in " + config_dir); return null;
	} else {
	  grch_out("Internal error. cannot read config file"); return null;
	}
      }
      Problem problem = new Problem();
      problem.width = challenge_data.w = ans_f.w;
      problem.height = challenge_data.h = ans_f.h;

      if (stage == P_HONSEN) {
	Date log_date = new Date();
	DateFormat df =
	  new SimpleDateFormat("MMM dd HH:mm:ss", Locale.ENGLISH);
	String env_user = System.getProperty("user.name");
	StringBuffer buf = new StringBuffer(100);
	buf.append(df.format(log_date) + ", " + env_user + "(---), problem "
		   + Integer.toString(problem_number) + " start\n");
	if (!logging_honsen(buf.toString())) {
	  grch_out("Internal error"); return null;
	}
      }
      
      Date start_date = new Date();
      challenge_data.start_time = start_date.getTime();
      String prob_key_str = make_problem_key(problem_number);
      //      System.out.println(prob_key_str);
      problem.key_string =
	file_encoding_key(problem_number) + "-" + prob_key_str;
      challenge_data.prob_num = problem_number;

      grch_out("Problem " + problem_number + ": Started");
      return problem;
    }
    catch (Exception e) {
      grch_out("Internal error: " + e);
      return null;
    }
  }
  public static String
  get_problem_files(int problem_number, String key_string,
		    String problem_dir_org) {
    int stage = P_INVALID;
    if ((stage = problem_number_check(problem_number)) == P_INVALID) {
      grch_out("problem_number is wrong"); return null;
    }
    //XXX todo: check otameshi, yosen, kessyou

    // check key_string
    if (!check_problem_key(key_string)) {
      grch_out("key_string is wrong"); return null;
    }
    String problem_dir = get_problem_dir(problem_number, problem_dir_org);
    if (problem_dir == null || problem_dir.length() == 0) {
      grch_out("problem_dir is wrong"); return null;
    }
    StringBuffer prob_dir = new StringBuffer(100);
    prob_dir.append(problem_dir);
    if (prob_dir.charAt(prob_dir.length() - 1) != '/') {
      prob_dir.append('/');
    }

    File dp = new File(prob_dir.toString());
    if (!dp.exists() || !dp.canRead() || !dp.isDirectory()) {
      if (stage == P_OTAMESHI) {
	grch_out("cannot open problem_dir :" + problem_dir); return null;
      } else {
	grch_out("cannot open problem_dir"); return null;
      }
    }
    File[] files = dp.listFiles();
    StringBuffer ret_files = new StringBuffer(files.length * 30);
    for (int i = 0; i < files.length; i++) {
      String fname = files[i].getAbsolutePath();
      if (!((fname.lastIndexOf(".mdt") == fname.length() - 4) ||
	    (fname.lastIndexOf(".rdt") == fname.length() - 4)))
	continue;
      ret_files.append(fname + ":");
    }
    return ret_files.toString();
  }
  public static final int GRCH_CORRECT = 0;
  public static final int GRCH_OK = 1;
  public static final int GRCH_WRONG = 2;
  public static final int GRCH_NG = 3;
  public static int
  answer_problem(int problem_number, String key_string,
		 int result,
		 String file_coordinates,
		 String config_dir_org) {
    Date end_date = new Date();
    int stage = P_INVALID;
    if ((stage = problem_number_check(problem_number)) == P_INVALID) {
      grch_out("problem_number is wrong"); return GRCH_NG;
    }
    if (stage == P_HONSEN) {
      DateFormat df =
	new SimpleDateFormat("MMM dd HH:mm:ss", Locale.ENGLISH);
      String env_user = System.getProperty("user.name");
      StringBuffer buf = new StringBuffer(100);
      buf.append(df.format(end_date) + ", " + env_user + "(---), problem "
		 + Integer.toString(problem_number) + " finish\n");
      if (!logging_honsen(buf.toString())) {
	grch_out("Internal error"); return GRCH_NG;
      }
    }
    if (challenge_data.prob_num != problem_number) {
      grch_out("problem_number changed"); return GRCH_NG;
    }
    int w_x = challenge_data.w;
    int w_y = challenge_data.h;
    // file_coordinatesがそろっているかチェック
    if (file_coordinates.length() < w_x * w_y) {
      grch_out("file_coordinates is wrong"); return GRCH_NG;
    }
    for (int i = 0; i < w_x * w_y; i++) {
      if (!(file_coordinates.charAt(i) == '0' ||
	    file_coordinates.charAt(i) == '1')) {
	grch_out("file_coordinates is wrong"); return GRCH_NG;
      }
    }

    // key check
    String prob_key_str = make_problem_key(problem_number);
    String correct_key_buf =
      file_encoding_key(problem_number) + "-" + prob_key_str;
    if (correct_key_buf.compareTo(key_string) != 0) {
      grch_out("key_string changed"); return GRCH_NG;
    }

    String config_dir = get_problem_dir(problem_number, config_dir_org);
    String config_str = answer_fname(config_dir, problem_number);
    if (config_str == null) {
      grch_out("config_dir is wrong"); return GRCH_NG;
    }
    Answer_file ans_f = new Answer_file();
    if (!ans_f.open(config_str)) {
      if (stage == P_OTAMESHI) {
	grch_out("cannot find config file in " + config_dir); return GRCH_NG;
      } else {
	grch_out("Internal error. cannot find config file"); return GRCH_NG;
      }
    }
    if (!ans_f.read_header()) {
      if (stage == P_OTAMESHI) {
	grch_out("cannot read config file in " + config_dir); return GRCH_NG;
      } else {
	grch_out("Internal error. cannot read config file"); return GRCH_NG;
      }
    }
    if (problem_number != ans_f.prob_num ||
	w_x != ans_f.w || w_y != ans_f.h) {
      if (stage == P_OTAMESHI) {
	grch_out("cannot read config file in " + config_dir); return GRCH_NG;
      } else {
	grch_out("Internal error. cannot read config file"); return GRCH_NG;
      }
    }
    part_ans ans[] = new part_ans[w_x * w_y];
    for (int i = 0; i < w_x * w_y; i++) {
      ans[i] = new part_ans();
    }
    if (!ans_f.read_body(ans)) {
      if (stage == P_OTAMESHI) {
	grch_out("cannot read config file in " + config_dir); return GRCH_NG;
      } else {
	grch_out("Internal error. cannot read config file"); return GRCH_NG;
      }
    }

    int correct_ans = 0;
    boolean lack = false;
    for (int x = 0; x < w_x; x++) {
      for (int y = 0; y < w_y; y++) {
	int ca = coord_a(x, y, w_x);
	if (file_coordinates.charAt(ca) == '0') {
	  lack = true; continue;
	}
	correct_ans += ans[ca].total;
	if (x != 0 && file_coordinates.charAt(coord_a(x - 1, y, w_x)) != '0')
	  correct_ans -= ans[ca].left;
	if (y != 0 && file_coordinates.charAt(coord_a(x, y - 1, w_x)) != '0')
	  correct_ans -= ans[ca].upper;
      }
    }

    ans = null;

    NumberFormat fmt = NumberFormat.getInstance();
    fmt.setMinimumIntegerDigits(2);
    fmt.setMaximumIntegerDigits(2);
    long msec = end_date.getTime() - challenge_data.start_time;
    long msec_sec = msec / 1000;
    long msec_msec = (msec / 10) % 100;
    grch_out("Problem " + problem_number + ": Finished");
    grch_out("Elapsed time is " + msec_sec + "."
	     + fmt.format(msec_msec) + "sec");

    int retval;
    String retstr[] = {"correct.",
		       "correct, but you did not count all files. To be qualified, you should count all.",
		       "wrong."};
    
    if (correct_ans == result) {
      if (lack) {
	retval = GRCH_OK;
      } else {
	retval = GRCH_CORRECT;
      }
    } else {
      retval = GRCH_WRONG;
    }
    grch_out("Your answer is " + retstr[retval]);
    
    if (retval == GRCH_CORRECT) {
    //    if (true) {

      StringBuffer buf = new StringBuffer(100);
      buf.append("Correct, prob ");
      buf.append(fmt.format(problem_number) + ", "
		 + msec_sec + "." + fmt.format(msec_msec) + "sec, at ");
      
      Date start_time = new Date(challenge_data.start_time);
      DateFormat df =
	new SimpleDateFormat("EEE MMM dd HH:mm:ss yyyy", Locale.ENGLISH);
      buf.append(df.format(start_time) + ".");
      String buf_str = buf.toString();
      byte md5_str[] = make_secret_md5(buf_str, buf_str.length());
      String md5_readable_buf = readable_enc(md5_str, 16);
      grch_out("<<" + buf_str + " / key = " + md5_readable_buf + ">>");
    }

    return retval;
  }

  private static final int P_INVALID = 0;
  private static final int P_OTAMESHI = 1;
  private static final int P_YOSEN = 2;
  private static final int P_HONSEN = 3;
  private static int problem_number_check(int pnum) {
    if (0 < pnum && pnum <= 10) return P_OTAMESHI;
    else if (11 <= pnum && pnum <= 20) return P_YOSEN;
    else if (21 <= pnum && pnum <= 26) return P_HONSEN;
    return P_INVALID;
  }
  private static String get_problem_dir(int prob_num,
					String user_problem_dir) {
    int stage = problem_number_check(prob_num);
    if (stage == P_YOSEN) { // 予選
      String test_dir = "/home/endo/yd/xet/";
      File test_dp = new File(test_dir);
      if (!test_dp.exists() || !test_dp.isDirectory()) {
	test_dir = "/usr/users/endo/yd/xet/";
	test_dp = new File(test_dir);
	if (!test_dp.exists() || !test_dp.isDirectory()) return null;
      }
      StringBuffer ret_dir = new StringBuffer(100);
      ret_dir.append(test_dir);
      NumberFormat fmt = NumberFormat.getInstance();
      fmt.setMinimumIntegerDigits(2);
      fmt.setMaximumIntegerDigits(2);
      ret_dir.append(fmt.format(prob_num));
      return ret_dir.toString();
    } else if (stage == P_HONSEN) { // 本選
      String test_dir = "/home/endo/kd/avaj/";
      File test_dp = new File(test_dir);
      if (!test_dp.exists() || !test_dp.isDirectory()) {
	test_dir = "/usr/users/endo/kd/avaj/";
	test_dp = new File(test_dir);
	if (!test_dp.exists() || !test_dp.isDirectory()) return null;
      }
      StringBuffer ret_dir = new StringBuffer(100);
      ret_dir.append(test_dir);
      NumberFormat fmt = NumberFormat.getInstance();
      fmt.setMinimumIntegerDigits(2);
      fmt.setMaximumIntegerDigits(2);
      ret_dir.append(fmt.format(prob_num));
      return ret_dir.toString();
    } else {
      return user_problem_dir;
    }
  }
  private static void grch_out(String format) {
    System.err.println("[Grid Challenge] " + format);
  }
  private static String make_problem_key(int prob_num) {
    StringBuffer time_str_buf = new StringBuffer();
    NumberFormat fmt = NumberFormat.getInstance();
    fmt.setMinimumIntegerDigits(2);
    fmt.setMaximumIntegerDigits(2);
    time_str_buf.append(fmt.format(prob_num));
    int cur_time =
      (int)((challenge_data.start_time - challenge_data.base_time) / 1000);
    StringBuffer ct_buf = new StringBuffer("00000000");
    String cur_time_str = Integer.toHexString(cur_time);
    ct_buf.replace(8 - cur_time_str.length(), 8, cur_time_str);
    time_str_buf.append(ct_buf);
    byte md5_str[] = make_secret_md5(time_str_buf.toString(), 10);
    if (md5_str == null) return null;
    return (time_str_buf + readable_enc(md5_str, 16));
  }
  private static boolean check_problem_key(String problem_key) {
    // XXX todo?: '-'以前のチェック
  
    // check after '-'
    int mp = problem_key.indexOf('-');
    if (mp == -1) return false;
    mp++;
    byte[] md5_str = make_secret_md5(problem_key.substring(mp), 10);
    String md5_readable_buf = readable_enc(md5_str, 16);
    if (md5_readable_buf.compareTo(problem_key.substring(mp + 10)) == 0) {
      return true;
    } else {
      return false;
    }
  }

  private static byte[] make_secret_md5(String data, int len) {
    try {
      java.security.MessageDigest md = 
	java.security.MessageDigest.getInstance("MD5");
      md.reset();
      md.update(data.getBytes(), 0, len);
      final String secret_key = new String("Internal error: ");
      md.update(secret_key.getBytes());
      return md.digest();
    }
    catch (java.security.NoSuchAlgorithmException e) {
      grch_out("Internal error: " + e);
      return null;
    }
    catch (Exception e) {
      grch_out("Internal error: " + e);
      return null;
    }
  }
  private static String answer_fname(String config_dir, int pnum) {
    if (config_dir == null || config_dir.length() == 0) return null;
    StringBuffer ans_fname = new StringBuffer(100);
    ans_fname.append(config_dir);
    if (ans_fname.charAt(ans_fname.length() - 1) != '/') {
      ans_fname.append('/');
    }
    NumberFormat fmt = NumberFormat.getInstance();
    fmt.setMinimumIntegerDigits(2);
    fmt.setMaximumIntegerDigits(2);
    ans_fname.append(fmt.format(pnum));
    ans_fname.append(".ans");
    return ans_fname.toString();
  }
  private static String readable_enc(byte[] str, int len) {
    StringBuffer ret = new StringBuffer();
    int end = len;
    if (str.length < len) end = str.length;
    for (int i = 0; i < end; i++) {
      char b1 = (char)((int)((str[i] >> 4) & 0x0f) + (int)('a'));
      ret.append(b1);
      char b2 = (char)((int)( str[i]       & 0x0f) + (int)('a'));
      ret.append(b2);
    }
    return ret.toString();
  }
  private static boolean logging_honsen(String buf) {
    String test_dir = "/home/endo/kd/loglog/";
    File test_dp = new File(test_dir);
    if (!test_dp.exists() || !test_dp.isDirectory()) {
      test_dir = "/usr/users/endo/kd/loglog/";
      test_dp = new File(test_dir);
      if (!test_dp.exists() || !test_dp.isDirectory()) return false;
    }
    String logfname = new String(test_dir + "finalstage.log");
    try {
      FileChannel logfc = new RandomAccessFile(logfname, "rw").getChannel();
      FileLock loglock = logfc.lock();
      if (logfc.size() == 0) {
	Runtime.getRuntime().exec("chmod 666 " + logfname);
      }
      logfc.position(logfc.size());
      byte[] buf_bytes = buf.getBytes();
      ByteBuffer buf_bb = ByteBuffer.wrap(buf_bytes);
      logfc.write(buf_bb);
      loglock.release();
      logfc.close();
    }
    catch (FileNotFoundException e) {
      return false;
    }
    catch (IOException e) {
      return false;
    }
    return true;
  }
  private static String file_encoding_key(int prob_num) {
    String str;
    switch (prob_num) {
    case 1:
      str = 
	"c9988f6c2fcb4d492baece7fe9e52a3796e5e6fa";
      break;
    case 2:
      str =
	"267aeade4ada4933fa7bbf106885e963e03d46d9";
      break;
    case 3:
      str = 
	"07acb0527fc8341f727b6ac77b56815ae25cef46";
      break;
    case 4:
      str = 
	"b41283d2d4ca5e6e3259b7c01088e57b2e80f77a";
      break;
    case 5:
      str = 
	"fdea1c3b65a51f307d7f465ad06e5a3ea1085f92";
      break;
    case 6:
      str = 
	"e02750f448b185705a23996389418df770f24024";
      break;
  case 11:
    str = 
      "719ca94cb2546a952483c6dbbe6981d2cdd5c1b6";
    break;
  case 12:
    str = 
      "307ba680ad0e78a6e3ee60b2ba1ee8ccafdd3005";
    break;
  case 13:
    str = 
      "507688d1bc5db09d71c942fa7ccbd12fa419e511";
    break;
  case 14:
    str = 
      "c602b97864f28f9097468c71d9ac1021fb36c084";
    break;
  case 15:
    str = 
      "e599f350b0c33aeb0c5f5b15e35f4ba6824808c2";
    break;
  case 16:
    str = 
      "6efa6bd45638a1ac12b757be1d5c3f2a61c39d96";
    break;
  case 17:
    str = 
      "7d1b577431c90e8488a36a81973c965b0d6a85d7";
    break;
  case 18:
    str = 
      "d8795092af64980aa5d290666148866b3ab5c9d3";
    break;
  case 19:
    str = 
      "f040bd9dfd5eb5dd9849cedf655e2e34876e0f3d";
    break;
  case 20:
    str = 
      "890807f633a661e4de2dbe393d3b9b47e5a1a131";
    break;
  case 21:
    str = 
      "d68c61cf8f2ad458a1482ec019a4b16ee5250131";
    break;
  case 22:
    str = 
      "a052a97e6b9b67eb91d0ce08744441ab97404c66";
    break;
  case 23:
    str = 
      "e44ad7a398df6eaf1096a7bd7d9fa6618ad1df61";
    break;
  case 24:
    str = 
      "1efa45d10fb28dc078e1a4f4734f121e4ff00d46";
    break;
  case 25:
    str = 
      "8509e0fcec9581ebfe025a3547a6a2e74ab93eae";
    break;
  case 26:
    str = 
      "454053f52b8703bbbc0001600ee349f74ace811f";
    break;
    default:
      if (prob_num <= 10) {
	str = "00000000";
      } else {
	grch_out("Unknown problem no");
	str = "";
      }
      break;
    }
    return str;
  }
}
