import { Navigate } from '@/api/navigate'
import { defineStore } from 'pinia'
import { ref } from 'vue'

export const useNavigateStore = defineStore('navigate', () => {
    const rdata = ref(null) // 需要返回给view的数据
    
    const GetNavigatePath = async (startId, endId) => {
        try {
            const res = await Navigate(startId, endId)
            console.log(res.data)

            if (res.data.ststus_code == 0) {
                // rdata.value = res.data
                return true
            }
            return false
        } catch (err) {
            console.log('记录')
            return false
        }
    }

    return {
        rdata,
        GetNavigatePath
    }
})

