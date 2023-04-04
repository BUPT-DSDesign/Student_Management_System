import { Navigate, TSP } from '@/api/navigate'
import { defineStore } from 'pinia'
import { ref } from 'vue'
import { Loading } from 'element-ui'

export const useNavigateStore = defineStore('navigate', () => {
    const startId = ref(-1);
    const endId = ref(-1);
    const rdata = ref(null) // 需要返回给view的数据
    const tspPath = ref(null) 
    const tspLoading = ref(null)
    const GetNavigatePath = async (startId, endId) => {
        try {
            const res = await Navigate(startId, endId)
            
            if (res.data.status_code == 0) {
                rdata.value = res.data
                return true
            }
            return false
        } catch (err) {
            return false
        }
    }

    const GetTSPPath = async (startId, passIds) => {
        tspLoading.value = Loading.service({
            lock: true,
            text: '寻路中，请稍候.....',
            background: 'rgba(0, 0, 0, 0.7)'
        })
        try {
            const res = await TSP(startId, passIds)

            if (res.data.status_code == 0) {
                tspPath.value = res.data
                tspLoading.value.close()
                return true
            }
            tspLoading.value.close()
            return false
        } catch (err) {
            tspLoading.value.close()
            return false
        }
    }

    return {
        rdata,
        tspPath,
        GetNavigatePath,
        GetTSPPath,
        startId,
        endId,
    }
},{
    persist: true
})

