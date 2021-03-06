#include "hadoop/Pipes.hh"
#include "hadoop/TemplateFactory.hh"
#include "hadoop/StringUtils.hh"

// 100G的数据，分别保存在5个文件中
// 5个数据文件的内容均为相同的格式
// 文件的内容大致如下:
// ABCDSDFKJDKF kkk 2890876
// SDKFJIEORUEW nnn 1231238
// LSFKDFSSDRDE bbb 9234999
// 每一行内容中，首先是一个12字节的字符串，
// 然后是一个3字节的字符串，
// 然后是一个7个数字组成的字符串。
// 字符串之间是用空格分隔的
//
// 问题:
// 在这100G的数据中，请统计出第二项字符串为"kkk"和"nnn"的数据分别有多少条�
// 

typedef std::vector<std::string> StringList;
class WordCountMap : public HadoopPipes::Mapper 
{
public:
  WordCountMap(HadoopPipes::TaskContext&context){}
  void map(HadoopPipes::MapContext&context) 
  {
    // 这里是分割字符串，每一行数据中的各项是以空格来分割的,
    // 分割的结果保存到了一个std::vector中
    // 例如: ABCDSDFKJDKF kkk 2890876
    // 其中contextWords[1]即为要统计
    StringList contextWords = HadoopUtils::splitString(context.getInputValue()," ");
    
    if("kkk"== contextWords[1])
    {
      context.emit("kkk","1");
    }
    else if("nnn"== contextWords[1]) 
    {
      context.emit("nnn","1");
    }
    
  }
};
 
class WordCountReduce : public HadoopPipes::Reducer 
{
public:
  WordCountReduce(HadoopPipes::TaskContext&context){}
  void reduce(HadoopPipes::ReduceContext&context)
  {
    int sum=0;
    while(context.nextValue()) 
    {
      sum++;
    }
    context.emit(context.getInputKey(), HadoopUtils::toString(sum));
  }
};
 
int main(int argc,char**argv)
{
  return HadoopPipes::runTask(HadoopPipes::TemplateFactory<WordCountMap, WordCountReduce>());
}
