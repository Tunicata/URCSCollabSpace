with Ada.Text_IO; use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Ada.Numerics.Elementary_Functions; use Ada.Numerics.Elementary_Functions;
with Ada.Containers.Vectors; with Ada.Containers.Ordered_Sets; use Ada.Containers;

procedure Main is 
   package Integer_Vectors is new Ada.Containers.Vectors
     (Index_Type   => Natural,
      Element_Type => Integer);

   use Integer_Vectors;

   function isPrime (Value : in Integer) return Boolean is
   begin
      for I in 2 .. (Integer(Sqrt(Float(Value)))) loop
         if Value mod I = 0 then
            return False;
         end if;
      end loop;
      return True;
   end isPrime;

   function primeGenerator (Num : Positive) return Vector is
      Primes: Vector;
      CurrFi: Integer;
      CurrSe: Integer;
   begin
      case Num is
      when 1 =>
         return Primes;
      when 2 =>
         Primes.Append(2);
         return Primes; 
      when 3..4 =>
         Primes := 2 & 3;
         return Primes;
      when others =>
         CurrFi := 5;
         CurrSe := 7;
         Primes := 2 & 3;

         loop
            if CurrFi > Num then
               return Primes;
            elsif isPrime(CurrFi) then
               Primes.Append(CurrFi);
            end if;
            CurrFi := CurrFi + 6;
            if CurrSe > Num then
               return Primes;
            elsif isPrime(CurrSe) then
               Primes.Append(CurrSe);
            end if;
            CurrSe := CurrSe + 6;
         end loop;
      end case;
   end primeGenerator;

   function primePartition (Num : Positive) return Vector is
      Half : Integer := Num / 2;
      Rest : Integer := Integer(Num);
      Curr : Integer;
      Limit : Integer;
      Sample : Vector := primeGenerator(Num);
      Lst : Vector;
      Ptr : Vector;
      Output : Vector;
   begin
      if Num > 1 then
         Lst.Append(2);
         Ptr.Append(0);
         Rest := Rest - 2;
         Limit := Integer(Sample.Length) - 1;

         loop
            if Rest <= 0 then
               if Rest = 0 then
                  for Prime of Lst loop
                     Output.Append(Prime);
                  end loop;
                  Output.Append(0);
               end if;

               Rest := Rest + Lst.element(Integer(Lst.Length)-1);
               Lst.Delete(Integer(Lst.Length)-1);
               Ptr.Delete(Integer(Ptr.Length)-1);

               Rest := Rest + Lst.element(Integer(Lst.Length)-1);
               Curr := Ptr.Last_Element + 1;
               Lst.Delete(Integer(Lst.Length)-1);
               Ptr.Delete(Integer(Ptr.Length)-1);
            else
               Curr := Ptr.Last_Element + 1;
            end if;

            if Curr > Limit then
               Rest := Rest + Lst.element(Integer(Lst.Length)-1);
               Lst.Delete(Integer(Lst.Length)-1);
               Ptr.Delete(Integer(Ptr.Length)-1);

               Rest := Rest + Lst.element(Integer(Lst.Length)-1);
               Curr := Ptr.Last_Element + 1;
               Lst.Delete(Integer(Lst.Length)-1);
               Ptr.Delete(Integer(Ptr.Length)-1);
            end if;

            Lst.Append(Sample.element(Curr));
            Ptr.Append(Curr);
            Rest := Rest - Lst.element(Integer(Lst.Length)-1);

            exit when Lst.First_Element > Half;
         end loop;
      end if;
      return Output;
   end primePartition;

   function Get_String return String is
      Line : String (1 .. 1_000);
      Last : Natural;
   begin
      Get_Line (Line, Last);
      return Line (1 .. Last);
   end Get_String;

   function Get_Integer return Integer is
      S : constant String := Get_String;
   begin
      return Integer'Value (S);
   end Get_Integer;

      Partitions : Vector;
      Num : Integer;
      Flag : Boolean := True;

begin
   Put("Please enter an Integer Larger than 1 to find its prime partitions: ");
   Num := Get_Integer;
   Partitions := primePartition(Num);
   for E of Partitions loop
      if E = 0 then
         Put_Line("");
         Flag := True;
      elsif Flag then
         Put(Integer'Image (E));
      else
         Put(Integer'Image (E) & " +");
      end if;
   end loop;
end Main;