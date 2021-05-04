package login;


public class RegistrationRequest extends Request{

    public RegistrationRequest(String userName,String userPassword){
        super(Command.registrationRequest,userName,userPassword);
    }
    public static byte[] WholeMsg(Command cmd,String userName,String userPassword)
            throws RePException{
        return new login.LoginRequest(userName, userPassword).getBytes();
    }
}
