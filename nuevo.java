import java.io.IOException;
import java.util.StringTokenizer;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class WordCount {

public static Text maxWord=new Text();
public static Text maxWord2=new Text();

  public static class TokenizerMapper
       extends Mapper<Object, Text, Text, IntWritable>{

    private final static IntWritable one = new IntWritable(1);
    private Text word = new Text();

    public void map(Object key, Text value, Context context
                    ) throws IOException, InterruptedException {
      StringTokenizer itr = new StringTokenizer(value.toString(),"/t");

        word.set(itr.nextToken());
        context.write(word, one);

    }
  }
  
  public static class IntSumReducer
       extends Reducer<Text,IntWritable,Text,IntWritable> {
    private IntWritable result = new IntWritable();
    private int max=0;
    public void reduce(Text key, Iterable<IntWritable> values,
                       Context context
                       ) throws IOException, InterruptedException {
     
      
      
      int sum = 0;
      for (IntWritable val : values) {
        sum += val.get();
      }

      if(sum>max)
      {
      max=sum;
      maxWord.set(key);
       }
}
}

}

  public static class TokenizerMapper2
       extends Mapper<Object, Text, Text, IntWritable>{

    private final static IntWritable one = new IntWritable(1);
    private Text word = new Text();
    String year ="nollave";
    int avgprice =0;

    public void map2(Object key, Text value, Context context
                    ) throws IOException, InterruptedException {
      StringTokenizer itr = new StringTokenizer(value.toString(),"/t");
        
        
        if(maxWord.equals.(word.set(itr.nextToken()))){
          
        for (int i=0; i<4;i++){	 word.set(itr.nextToken()); }
		    context.write(word, one);
        }

        context.write(new Text(year), new IntWritable(avgprice));

    }
  }
  

  
  public static class IntSumReducer2
       extends Reducer<Text,IntWritable,Text,IntWritable> {
    private IntWritable result = new IntWritable();
    private int max=0;
    public void reduce2(Text key, Iterable<IntWritable> values,
                       Context context
                       ) throws IOException, InterruptedException {
      int sum = 0;
      for (IntWritable val : values) {
        sum += val.get();
      }

      if(sum>max)
      {
      max=sum;
      maxWord2.set(key);
       }
}
}

protected void cleanup(Context context) throws IOException, InterruptedException{
    context.write(maxWord2, new IntWritable(max));
}
}

  public static void main(String[] args) throws Exception {
    Configuration conf = new Configuration();
    Job job = Job.getInstance(conf, "word count");
    job.setJarByClass(WordCount.class);
    job.setMapperClass(TokenizerMapper.class);
    job.setCombinerClass(IntSumReducer.class);
    job.setReducerClass(IntSumReducer.class);
    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(IntWritable.class);
    FileInputFormat.addInputPath(job, new Path(args[0]));
    
    Configuration conf2 = new Configuration();
    Job job2 = Job.getInstance(conf2, "word count2");
    job2.setJarByClass(WordCount.class);
    job2.setMapperClass(TokenizerMapper2.class);
    job2.setCombinerClass(IntSumReducer2.class);
    job2.setReducerClass(IntSumReducer2.class);
    job2.setOutputKeyClass(Text.class);
    job2.setOutputValueClass(IntWritable.class);
    FileInputFormat.addInputPath(job2, new Path(args[0]));
    FileOutputFormat.setOutputPath(job2, new Path(args[1]));
    System.exit(job.waitForCompletion(true) ? 0 : 1);

  }
}
