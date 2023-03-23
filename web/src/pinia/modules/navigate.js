import { Navigate, TSP } from '@/api/navigate'
import { defineStore } from 'pinia'
import { ref } from 'vue'

export const useNavigateStore = defineStore('navigate', () => {
    const rdata = ref(null) // 需要返回给view的数据
    const tspPath = ref(null) 
    
    const GetNavigatePath = async (startId, endId) => {
        try {
            const res = await Navigate(startId, endId)
            
            if (res.data.status_code == 0) {
                rdata.value = res.data
                return true
            }
            return false
        } catch (err) {
            console.log('记录')
            return false
        }
    }

    const GetTSPPath = async (startId, passIds) => {
        try {
            const res = await TSP(startId, passIds)

            if (res.data.status_code == 0) {
                tspPath.value = res.data
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
        tspPath,
        GetNavigatePath,
        GetTSPPath
    }
})

