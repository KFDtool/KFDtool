using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HidLibrary
{
    public class HidAsyncState
    {
        private readonly object _callerDelegate;

        private readonly object _callbackDelegate;

        public object CallerDelegate => _callerDelegate;

        public object CallbackDelegate => _callbackDelegate;

        public HidAsyncState(object callerDelegate, object callbackDelegate)
        {
            _callerDelegate = callerDelegate;
            _callbackDelegate = callbackDelegate;
        }
    }
}
