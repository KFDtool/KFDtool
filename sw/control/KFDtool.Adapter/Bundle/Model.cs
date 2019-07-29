using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.Adapter.Bundle
{
    public class Model
    {
        public string Name { get; set; }

        public List<string> Revision { get; set; }

        public Model()
        {
            Revision = new List<string>();
        }
    }
}
