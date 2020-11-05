
namespace PGApi
{
    public class Cell
    {
        public byte type;

        public Cell(byte value = 0)
        {
            type = (byte)(value & 0x0f);
        }

        //public void AddFreedom()
        //{
        //    type = (byte) Random.Rand((uint)(type + 1), 16);
        //}

        public static Cell SetTypeCell(Cell[,] buffer, int rows, int columns, int x, int y)
        {
            Cell cell = new Cell((byte)(x > 0 ? buffer[x - 1, y] != null ? 1 : 0 : 0));
            cell.type = (byte)((cell.type << 1) + (rows > x ? buffer[x + 1, y] != null ? 1 : 0 : 0));
            cell.type = (byte)((cell.type << 1) + (columns > y ? buffer[x, y + 1] != null ? 1 : 0 : 0));
            cell.type = (byte)((cell.type << 1) + (y > 0 ? buffer[x, y - 1] != null ? 1 : 0 : 0));
            return cell;
        }

        public static void SetCells(Cell[,] buffer, int rows, int columns)
        {
            if (buffer == null)
            {
                return;
            }

            if (rows < 2)
            {
                return;
            }

            if (columns < 2)
            {
                return;
            }

            // Set the edges
            if (buffer[0, 0] != null)
            {
                buffer[0, 0].type = (byte)(((buffer[0, 1] != null ? 1 : 0) << 1) + ((buffer[1, 0] != null ? 1 : 0) << 2));
            }
            if (buffer[0, columns - 1] != null)
            {
                buffer[0, columns - 1].type = (byte)((buffer[0, columns - 2] != null ? 1 : 0) + ((buffer[1, columns - 1] != null ? 1 : 0) << 2));
            }
            if (buffer[rows - 1, 0] != null)
            {
                buffer[rows - 1, 0].type = (byte)(((buffer[rows - 2, 0] != null ? 1 : 0) << 3) + ((buffer[rows - 1, 1] != null ? 1 : 0) << 1));
            }
            if (buffer[rows - 1, columns - 1] != null)
            {
                buffer[rows - 1, columns - 1].type = (byte)((buffer[rows - 1, columns - 2] != null ? 1 : 0) + ((buffer[rows - 2, columns - 1] != null ? 1 : 0) << 3));
            }


            for (int i = 1; i < rows - 1; i++)
            {
                if (buffer[i, columns - 1] != null)
                {
                    buffer[i, columns - 1].type = (byte)(
                        (buffer[i, columns - 2] != null ? 1 : 0) +
                        ((buffer[i - 1, columns - 1] != null ? 1 : 0) << 3) +
                        ((buffer[i + 1, columns - 1] != null ? 1 : 0) << 2)
                        );
                }
                if (buffer[i, 0] != null)
                {
                    buffer[i, 0].type = (byte)(
                        ((buffer[i, 1] != null ? 1 : 0) << 1) +
                        ((buffer[i - 1, 1] != null ? 1 : 0) << 3) +
                        ((buffer[i + 1, 1] != null ? 1 : 0) << 2)
                        );
                }
            }

            for (int i = 1; i < columns - 1; i++)
            {
                if (buffer[rows - 1, i] != null)
                {
                    buffer[rows - 1, i].type = (byte)(
                        ((buffer[rows - 2, i] != null ? 1 : 0) << 3) +
                        (buffer[rows - 1, i - 1] != null ? 1 : 0) +
                        ((buffer[rows - 1, i + 1] != null ? 1 : 0) << 1)
                        );
                }
                if (buffer[0, i] != null)
                {
                    buffer[0, i].type = (byte)(
                        ((buffer[1, i] != null ? 1 : 0) << 3) +
                        (buffer[0, i - 1] != null ? 1 : 0) +
                        ((buffer[0, i + 1] != null ? 1 : 0) << 1)
                        );
                }
            }

            for (int i = 1, j = 1; i < rows - 1 && j < columns - 1; i++)
            {
                if (buffer[i, j] != null)
                {
                    buffer[i, j].type = (byte)(
                        (buffer[i, j - 1] != null ? 1 : 0) +
                        ((buffer[i, j + 1] != null ? 1 : 0) << 1) +
                        ((buffer[i - 1, j] != null ? 1 : 0) << 2) +
                        ((buffer[i + 1, j] != null ? 1 : 0) << 3)
                        );
                }
                j++;
            }
        }
    }

    // 0  = 0000 : 0 degrees of freedom ; 
    // 1  = 0001 : 1 degrees of freedom ; left
    // 2  = 0010 : 1 degrees of freedom ; right
    // 3  = 0011 : 2 degrees of freedom ; left and right
    // 4  = 0100 : 1 degrees of freedom ; down
    // 5  = 0101 : 2 degrees of freedom ; down and left
    // 6  = 0110 : 2 degrees of freedom ; down and right
    // 7  = 0111 : 3 degrees of freedom ; down and right and left
    // 8  = 1000 : 1 degrees of freedom ; up
    // 9  = 1001 : 2 degrees of freedom ; up and left
    // 10 = 1010 : 2 degrees of freedom ; up and right
    // 11 = 1011 : 3 degrees of freedom ; up and right and left
    // 12 = 1100 : 2 degrees of freedom ; up and down
    // 13 = 1101 : 3 degrees of freedom ; up and down and left
    // 14 = 1110 : 3 degrees of freedom ; up and down and right
    // 15 = 1111 : 4 degrees of freedom ; up and down and right and left
}