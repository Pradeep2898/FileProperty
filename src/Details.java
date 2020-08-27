
public class Details {
	native String[] returndays(String s);
    native String sayHello(String name);
    native void modifyACL(String file, String user, char val, int perm);
    
    static
   {
      System.loadLibrary("Details");
   }
  
   static public void main(String args[])
   {
      Details obj = new Details();
      String s = obj.sayHello("Pradeep");  // invoke the native method
      System.out.println("Hello, "+s);
      String[] days = obj.returndays("Rahul");
      System.out.println(" > The days of the week are :: ");
      for(String name: days)
        System.out.println(name);
   }
}
