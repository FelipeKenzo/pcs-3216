library ieee;
use ieee.numeric_bit.all;

entity resto is
    port (
        clock , reset : in bit ;
        inicio : in bit ;
        fim : out bit ;
        dividendo , divisor : in bit_vector (15 downto 0 ) ;
        resto : out bit_vector (15 downto 0)
    );
end resto ;

architecture calcResto of resto is
    signal dividendo1 : integer range 0 to 255;
    signal divisor1   : integer range 0 to 255;
 
begin
    process (clock, reset, inicio, dividendo1, divisor1, dividendo, divisor)
    begin
    if reset='1' then
        resto <= "0000000000000000";
        divisor1   <= to_integer(unsigned(divisor));
        dividendo1 <= to_integer(unsigned(dividendo));
    elsif clock'event and clock='1' then
        if inicio = '1' then
            if dividendo1 >= divisor1  then
                dividendo1 <= dividendo1 - divisor1;
            else
                resto <= bit_vector(to_unsigned(dividendo1, resto'length)); 
                fim <= '1';
            end if;
        end if;
    end if;

  end process;
end architecture calcResto;